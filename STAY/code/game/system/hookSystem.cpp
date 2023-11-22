#include "hookSystem.hpp"
#include "../component/player.hpp"
#include "../component/hook.hpp"
#include "../../common/physics/collider.hpp"
#include "../../common/physics/rigidBody.hpp"
#include "../../common/physics/joint.hpp"
#include "../../common/utility/math.hpp"
#include "../../common/physics/world.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <box2d/b2_math.h>
#include <box2d/box2d.h>
#include <iostream>/*debug*/

namespace 
{
    void createBoxConnect(const stay::Vector2& playerPosition, stay::Node* pin, float length, float margin);
} // namespace

namespace stay
{
    HookSystem::HookSystem(ecs::Manager* manager)
        : ecs::System(manager)
        , ecs::UpdateSystem(0)
        , ecs::InputSystem(-1)
    {}
    
    void HookSystem::update(float dt)
    {
        updateCooldown(dt);
        updateDirection();
        processQueue();

        auto view = mManager->getRegistryRef().view<Hook, phys::RigidBody, Player>();
        for (auto [entity, hook, rigidbody, player] : view.each())
        {
            switch (hook.status.state)
            {
                case Hook::NONE:
                    player.onRope = false;
                    break;
                case Hook::SHOT:
                {
                    const auto bulletPosition = hook.status.createdPin->getComponent<phys::RigidBody>().getPosition();
                    const auto playerPosition = rigidbody.getPosition();
                    const bool exceeded = utils::lengthVec2(bulletPosition - playerPosition) > hook.props.maxLength;
                    if (exceeded)
                        clearBullet(hook);
                    break;
                }
                case Hook::CONNECTED:
                    updateControl(hook, dt);
                    break;
            }         
        }
    }

    void HookSystem::input(const sf::Event& event)
    {
        const bool keyPressed = event.type == sf::Event::KeyPressed;
        const bool shot = keyPressed && event.key.scancode == sf::Keyboard::Scancode::X;
        const bool dispatch = keyPressed && event.key.scancode == sf::Keyboard::Scancode::Space;

        auto view = mManager->getRegistryRef().view<Hook, Player>();
        for (auto [entity, hook, player] : view.each())
        {
            switch (hook.status.state)
            {
                case Hook::NONE:
                    if (shot && hook.status.shootable)
                        generateBullet(hook);
                    break;
                case Hook::SHOT:
                    break;
                case Hook::CONNECTED:
                    if (dispatch)
                        clearBullet(hook);
                    player.canJump = true;
                    break;
            }         
        }
    }

    void HookSystem::updateDirection()
    {
        mDirection = Vector2();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            mDirection.x -= 1.F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            mDirection.x += 1.F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            mDirection.y += 1.F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            mDirection.y -= 1.F;
        if (mDirection == Vector2())
        {
            const Vector2 bestVector = vectorUp;
            mDirection = bestVector;
        }
        else
            mDirection = glm::normalize(mDirection);
    }

    void HookSystem::updateControl(Hook& hook, float dt)
    {
        int up = 0;
        up += sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up);
        up -= sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down);

        auto* prismatic = hook.getNode()->getComponent<phys::Joint>().getNativeHandle<b2PrismaticJoint>();
        float currentLength = prismatic->GetJointTranslation();
        currentLength -= up * hook.props.pullSpeed * dt;
        currentLength = std::min(hook.props.maxLength, currentLength);
        prismatic->SetLimits(0, currentLength);
    }

    void HookSystem::generateBullet(Hook& hook)
    {
        auto* playerNode = hook.getNode();
        auto& playerBody = playerNode->getComponent<phys::RigidBody>();
        auto* bullet = playerNode->createChild();
        auto& bulletBody = bullet->addComponent<phys::RigidBody>(playerBody.getPosition(), 45.F, phys::BodyType::DYNAMIC);
        auto& bulletCollider = bullet->addComponent<phys::Collider>(phys::Box{Vector2(), Vector2{0.2F, 0.2F}}, phys::Material{5.F, 1.F, 0.F});

        bulletBody.setBullet(true);
        bulletBody.setVelocity(mDirection * hook.props.speed);

        bulletCollider.start();
        bulletCollider.setLayer("Bullet");
        bulletCollider.OnCollisionEnter.addEventListener( [this, &hook](phys::Collision& contact) -> void {
            queueForAttachment(&hook, &contact.other->getNode()->getComponent<phys::RigidBody>());
        });

        hook.status.state = Hook::SHOT;
        hook.status.createdPin = bullet;
        hook.status.timeToCD = hook.props.cooldown;
        hook.status.shootable = false;
    }

    void HookSystem::clearBullet(Hook& hook)
    {
        hook.getNode()->destroy(hook.status.createdPin);
        if (hook.status.collisionChecker != nullptr)
            hook.getNode()->destroy(hook.status.collisionChecker);
        hook.status.createdPin = nullptr;
        hook.status.collisionChecker = nullptr;
        hook.status.state = Hook::NONE;
    }

    void HookSystem::updateCooldown(float dt)
    {
        auto view = mManager->getRegistryRef().view<Hook>();
        for (auto [entity, hook] : view.each())
        {
            hook.status.timeToCD -= dt;
            if (hook.status.timeToCD < 0.F)
            {
                hook.status.timeToCD = 0.F;
                hook.status.shootable = true;
            }
        }
    }

    void HookSystem::processQueue()
    {
        for (const auto [hook, obstacle] : mQueued)
        {
            auto* bullet = hook->status.createdPin;
            auto& bulletBody = bullet->getComponent<phys::RigidBody>();
            auto& bulletCollider = bullet->getComponent<phys::Collider>();
            auto& playerBody = hook->getNode()->getComponent<phys::RigidBody>();
            auto& playerCollider = hook->getNode()->getComponent<phys::Collider>();
            const auto playerPosition = playerBody.getPosition();
            const auto bulletPosition = bulletBody.getPosition();

            // Create joint to the pin
            auto& playerPrisJoint = hook->getNode()->addComponent<phys::Joint>();
            auto& bulletRevJoint = bullet->addComponent<phys::Joint>();
            
            // Set up the pin
            bulletCollider.setLayer("Isolate");
            playerPrisJoint.start(bullet->entity(), phys::Prismatic{playerPosition, bulletPosition, bulletPosition - playerPosition}, false);
            bulletRevJoint.start(obstacle->get(), phys::Revolute{bulletBody.getPosition()}, true);

            // Create collision checker
            createBoxConnect(playerPosition, bullet, hook->props.maxLength, 0.6F);
            
            auto* nativePrisJoint = playerPrisJoint.getNativeHandle<b2PrismaticJoint>();
            nativePrisJoint->EnableLimit(true);
            nativePrisJoint->SetLimits(0.F, hook->props.maxLength);

            hook->status.state = Hook::CONNECTED;
            hook->getNode()->getComponent<Player>().onRope = true;
        }
        mQueued.clear();
    }

    void HookSystem::queueForAttachment(Hook* hook, phys::RigidBody* body)
    {
        mQueued.emplace(hook, body);
    }
} // namespace stay

namespace 
{
    void createBoxConnect(const stay::Vector2& playerPosition, stay::Node* pin, float length, float margin) 
    {
        auto& pinBody = pin->getComponent<stay::phys::RigidBody>();
        auto pinPosition = pinBody.getPosition();

        auto direction = playerPosition - pinPosition;
        auto absoluteCenter = pinPosition + length /2.F * glm::normalize(direction);
        auto center = stay::utils::convertVec2<stay::Vector2>(
            pinBody.body()->GetLocalPoint(
                stay::utils::convertVec2<b2Vec2>(absoluteCenter)));
        auto angleToOx = std::abs(direction.x) < 1e-4 ? 90.F : std::atan2(direction.y, direction.x) * RAD2DEG;
        auto absoluteAngle = angleToOx - 90.F;
        auto angle = absoluteAngle - pinBody.getAngle();

        stay::phys::Box box{center, stay::Vector2{0.2F, length - 2 * margin}, angle};
        static stay::phys::Material mat{0.005F};
        auto& collider = pin->addComponent<stay::phys::Collider01>(box, mat);
        collider.setLayer("Player");
        collider.OnCollisionDetection.addEventListener([](stay::phys::Collision& /*contact*/, b2Contact& nativeInfo) {
            std::cout << "collided ey\t"; /*debug*/
            nativeInfo.SetEnabled(false);
        });
        collider.start();
    }
} // namespace
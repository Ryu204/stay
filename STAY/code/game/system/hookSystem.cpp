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

namespace 
{
    const stay::Vector2 bulletSize{0.5F, 0.5F};
    const stay::Vector2 pinSize{0.2F, 0.2F};
    const stay::phys::Material heavyMaterial{5.F};
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
        attachQueuedBullets();
        splitQueuedPins();

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
                    const auto bulletPosition = hook.status.bullet->getComponent<phys::RigidBody>().getPosition();
                    const auto playerPosition = rigidbody.getPosition();
                    const bool exceeded = utils::lengthVec2(bulletPosition - playerPosition) > hook.props.ropeLength;
                    if (exceeded)
                        resetHook(hook);
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
                        resetHook(hook);
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
        up += (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up);
        up -= (int)sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down);

        auto* prismatic = hook.getNode()->getComponent<phys::Joint>().getNativeHandle<b2PrismaticJoint>();
        float currentLength = prismatic->GetJointTranslation();
        currentLength -= up * hook.props.pullSpeed * dt;
        currentLength = std::min(hook.props.ropeLength, currentLength);
        prismatic->SetLimits(0, currentLength);
    }

    void HookSystem::generateBullet(Hook& hook)
    {
        auto* playerNode = hook.getNode();
        auto& playerBody = playerNode->getComponent<phys::RigidBody>();
        auto* bullet = playerNode->createChild();
        auto& bulletBody = bullet->addComponent<phys::RigidBody>(playerBody.getPosition(), 0.F, phys::BodyType::DYNAMIC);
        auto& bulletCollider = bullet->addComponent<phys::Collider>(phys::Box{Vector2(), bulletSize}, heavyMaterial);

        bulletBody.setBullet(true);
        bulletBody.setVelocity(mDirection * hook.props.speed);
        bulletCollider.setLayer("Bullet");
        bulletCollider.OnCollisionEnter.addEventListener( [this, &hook](phys::Collision& contact) -> void {
            queueForAttachment(&hook, &contact.other->getNode()->getComponent<phys::RigidBody>());
        });
        bulletCollider.start();

        hook.status.state = Hook::SHOT;
        hook.status.maxLength = hook.props.ropeLength;
        hook.status.bullet = bullet;
        hook.status.timeToCD = hook.props.cooldown;
        hook.status.shootable = false;
    }

    void HookSystem::resetHook(Hook& hook)
    {
        if (hook.status.bullet != nullptr)
        {
            hook.getNode()->destroy(hook.status.bullet);
            hook.status.bullet = nullptr;
        }

        for (auto* pin : hook.status.createdPins)
            hook.getNode()->destroy(pin);
        hook.status.createdPins.clear();

        if (hook.status.collisionChecker != nullptr)
        {
            hook.getNode()->destroy(hook.status.collisionChecker);
            hook.status.collisionChecker = nullptr;
        }

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

    void HookSystem::attachQueuedBullets()
    {
        for (const auto& [hook, obstacle] : mQueuedAttaches)
        {
            auto position = hook->status.bullet->getComponent<phys::RigidBody>().getPosition();
            // Delete the bullet
            hook->getNode()->destroy(hook->status.bullet);
            hook->status.bullet = nullptr;
            createPinAt(position, hook, obstacle->getNode());
            hook->getNode()->getComponent<Player>().onRope = true;
        }
        mQueuedAttaches.clear();
    }

    void HookSystem::splitQueuedPins() 
    {
        for (const auto& [hook, tup] : mQueuedSplitting)
        {
            const auto& [pos, plat] = tup;
            createPinAt(pos, hook, plat);
        }

        mQueuedSplitting.clear();
    }

    void HookSystem::queueForAttachment(Hook* hook, phys::RigidBody* obstacle)
    {
        mQueuedAttaches.emplace(hook, obstacle);
    }

    void HookSystem::createBoxConnect(Node* player, Node* pin, float length, float margin) 
    {
        auto& playerBody = player->getComponent<phys::RigidBody>();
        const auto playerPosition = playerBody.getPosition();
        auto& pinBody = pin->getComponent<phys::RigidBody>();
        auto pinPosition = pinBody.getPosition();

        auto direction = playerPosition - pinPosition;
        auto absoluteCenter = pinPosition + length / 2.F * glm::normalize(direction);
        auto center = utils::convertVec2<Vector2>(
            pinBody.body()->GetLocalPoint(
                utils::convertVec2<b2Vec2>(absoluteCenter)));
        auto angleToOx = std::abs(direction.x) < 1e-4 ? 90.F : std::atan2(direction.y, direction.x) * RAD2DEG;
        auto absoluteAngle = angleToOx - 90.F;
        auto angle = absoluteAngle - pinBody.getAngle();

        auto& basePinCollider = pin->addComponent<phys::Collider>(phys::Box{Vector2{}, pinSize}, phys::Material{5.F});
        basePinCollider.setLayer("Isolate");
        basePinCollider.start();

        phys::Box box{center, Vector2{0.2F, length - 2 * margin}, angle};
        static phys::Material mat{0.005F};
        auto& collider = pin->addComponent<phys::Collider01>(box, mat);
        collider.setLayer("Player");
        
        auto& hook = player->getComponent<Hook>();
        collider.OnCollisionDetection.addEventListener([this, &playerBody, &hook, pinPosition](phys::Collision& contact, b2Contact& nativeInfo) {
            const bool touchAtExtension = utils::lengthVec2(playerBody.getPosition() - pinPosition) < utils::lengthVec2(contact.position - pinPosition);
            if (!touchAtExtension)
                mQueuedSplitting.emplace(&hook, std::tuple<Vector2, Node*>(contact.position, contact.other->getNode()));
            nativeInfo.SetEnabled(false);
        });
        collider.start();
    }

    void HookSystem::createPinAt(const Vector2& position, Hook* hook, Node* platform)
    {
        auto* pin = hook->getNode()->createChild();
        auto& pinBody = pin->addComponent<phys::RigidBody>(position, 0.F, phys::BodyType::DYNAMIC);
        auto& playerBody = hook->getNode()->getComponent<phys::RigidBody>();
        const auto playerPosition = playerBody.getPosition();

        // Create joint to the pin
        if (!hook->status.createdPins.empty())
        {
            hook->getNode()->removeComponents<phys::Joint>();
            hook->status.createdPins.back()->removeComponents<phys::Collider01>();
        }
        auto& playerPrisJoint = hook->getNode()->addComponent<phys::Joint>();
        auto& bulletRevJoint = pin->addComponent<phys::Joint>();
        
        // Set up the pin
        playerPrisJoint.start(pin->entity(), phys::Prismatic{playerPosition, position, position - playerPosition}, false);
        bulletRevJoint.start(platform->entity(), phys::Revolute{position}, true);

        // Create collision checker
        createBoxConnect(hook->getNode(), pin, hook->status.maxLength, 0.6F);
        
        auto* nativePrisJoint = playerPrisJoint.getNativeHandle<b2PrismaticJoint>();
        nativePrisJoint->EnableLimit(true);
        nativePrisJoint->SetLimits(0.F, hook->status.maxLength);

        hook->status.state = Hook::CONNECTED;
        hook->status.createdPins.push_back(pin);
    }
} // namespace stay
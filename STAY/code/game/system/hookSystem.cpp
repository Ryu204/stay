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

        auto view = mManager->getRegistryRef().view<Hook, phys::RigidBody>();
        for (auto [entity, hook, rigidbody] : view.each())
        {
            switch (hook.status.state)
            {
                case Hook::NONE:
                    break;
                case Hook::SHOT:
                {
                    const auto bulletPosition = hook.status.created->getComponent<phys::RigidBody>().getPosition();
                    const auto playerPosition = rigidbody.getPosition();
                    const bool exceeded = utils::lengthVec2(bulletPosition - playerPosition) > hook.props.maxLength;
                    if (exceeded)
                        clearBullet(hook);
                    break;
                }
                case Hook::CONNECTED:
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
        hook.status.created = bullet;
        hook.status.timeToCD = hook.props.cooldown;
        hook.status.shootable = false;
    }

    void HookSystem::clearBullet(Hook& hook)
    {
        hook.getNode()->destroy(hook.status.created);
        hook.status.created = nullptr;
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
            auto* bullet = hook->status.created;
            auto& bulletBody = bullet->getComponent<phys::RigidBody>();
            auto& bulletCollider = bullet->getComponent<phys::Collider>();
            auto& playerBody = hook->getNode()->getComponent<phys::RigidBody>();
            auto& playerCollider = hook->getNode()->getComponent<phys::Collider>();
            const auto playerPosition = playerBody.getPosition();
            const auto bulletPosition = bulletBody.getPosition();
            auto& playerPrisJoint = hook->getNode()->addComponent<phys::Joint>();
            auto& bulletRevJoint = bullet->addComponent<phys::Joint>();
            
            bulletCollider.setLayer("Isolate");
            playerPrisJoint.start(bullet->entity(), phys::Prismatic{playerPosition, bulletPosition, bulletPosition - playerPosition}, false);
            bulletRevJoint.start(obstacle->get(), phys::Revolute{bulletBody.getPosition()}, false);
            
            auto* nativePrisJoint = playerPrisJoint.getNativeHandle<b2PrismaticJoint>();
            nativePrisJoint->EnableLimit(true);
            nativePrisJoint->SetLimits(0.F, hook->props.maxLength);
            nativePrisJoint->EnableMotor(true);
            nativePrisJoint->SetMotorSpeed(0.F);
            const float playerP = playerBody.gravityScale() * playerCollider.mass() * phys::World::get().GetGravity().y;
            nativePrisJoint->SetMaxMotorForce(std::abs(hook->props.stiffness * playerP));

            hook->status.state = Hook::CONNECTED;
        }
        mQueued.clear();
    }

    void HookSystem::queueForAttachment(Hook* hook, phys::RigidBody* body)
    {
        mQueued.emplace(hook, body);
    }
} // namespace stay
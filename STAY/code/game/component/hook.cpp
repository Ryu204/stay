#include "hook.hpp"
#include "../../common/physics/collider.hpp"
#include "../../common/physics/rigidBody.hpp"
#include "../../common/physics/joint.hpp"

namespace stay
{
    void Hook::reset()
    {
        if (created != nullptr)
        {
            getNode()->destroy(created);
            created = nullptr;
        }
    }

    HookSystem::HookSystem(ecs::Manager* manager)
        : ecs::System(manager)
        , ecs::UpdateSystem(0)
        , ecs::InputSystem(0)
    {}
    
    void HookSystem::update(float dt)
    {
        updateCooldown(dt);
        updateDirection();
        processQueue();
    }

    void HookSystem::input(const sf::Event& event)
    {
        if (event.type != sf::Event::KeyPressed || event.key.scancode != sf::Keyboard::Scancode::X)
            return;
        
        auto view = mManager->getRegistryRef().view<Hook>();
        for (auto [entity, hook] : view.each())
        {
            hook.reset();
            if (!hook.shootable)
                continue;
            generateBullet(hook);            
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
        auto* node = hook.getNode();
        auto& baseRg = node->getComponent<phys::RigidBody>();

        auto* bullet = node->createChild();
        auto& rg = bullet->addComponent<phys::RigidBody>(baseRg.getPosition(), 45.F, phys::BodyType::DYNAMIC);
        phys::Material mat(5.F, 1.F, 0.F);
        auto& col = bullet->addComponent<phys::Collider>(phys::Box{Vector2(), Vector2{0.2F, 0.2F}}, mat);
        col.start();
        col.setLayer("Bullet");
        rg.setBullet(true);
        rg.setVelocity(mDirection * hook.speed);
        col.OnCollisionEnter.addEventListener(
            [this, &hook](phys::Collision& contact)
            {
                bool generated = hook.getNode()->hasComponent<phys::Joint>();
                if (!generated)
                    queueForAttachment(&hook, &contact.other->getNode()->getComponent<phys::RigidBody>());
            });

        hook.created = bullet;
        hook.timeToCD = hook.cooldown;
        hook.shootable = false;
    }

    void HookSystem::updateCooldown(float dt)
    {
        auto view = mManager->getRegistryRef().view<Hook>();
        for (auto [entity, hook] : view.each())
        {
            hook.timeToCD -= dt;
            if (hook.timeToCD <= 0.F)
            {
                hook.timeToCD = 0.F;
                hook.shootable = true;
            }
        }
    }

    void HookSystem::processQueue()
    {
        for (const auto [hook, obstacle] : mQueued)
        {
            auto& bullet = hook->created->getComponent<phys::RigidBody>();
            auto& bulletCol = hook->created->getComponent<phys::Collider>();
            auto& base = hook->getNode()->getComponent<phys::RigidBody>();
            auto& slider = hook->getNode()->addComponent<phys::Joint>();
            auto& rotation = hook->created->addComponent<phys::Joint>();
            
            bulletCol.setLayer("Isolate");
            slider.start(
                hook->created->entity(), 
                phys::Prismatic{bullet.getPosition(), bullet.getPosition() - base.getPosition()}, 
                false);
            rotation.start(obstacle->get(), phys::Revolute{bullet.getPosition()}, false);
        }
        mQueued.clear();
    }
    void HookSystem::queueForAttachment(Hook* hook, phys::RigidBody* body)
    {
        mQueued.emplace(hook, body);
    }
} // namespace stay
#include "hook.hpp"
#include "../../common/physics/collider.hpp"
#include "../../common/physics/rigidBody.hpp"

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
        fixQueued();
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
            Vector2 bestVector = vectorUp;
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
        auto& col = bullet->addComponent<phys::Collider>(phys::Box{Vector2(), Vector2{0.2F, 0.2F}});
        col.start();
        col.setLayer("Bullet");
        rg.setBullet(true);
        rg.setVelocity(mDirection * hook.speed);
        col.OnCollisionEnter.addEventListener(
            [this, &hook](phys::Collider& /*collider*/, b2Contact& /*contact*/)
            {
                addToFixQueue(hook);
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

    void HookSystem::fixQueued()
    {
        for (const auto& hook : mQueueFixing)
        {
            hook->created->getComponent<phys::RigidBody>().setType(phys::BodyType::STATIC);
        }
        mQueueFixing.clear();
    }
    void HookSystem::addToFixQueue(Hook& hook)
    {
        mQueueFixing.emplace(&hook);
    }
} // namespace stay
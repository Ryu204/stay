#include "hook.hpp"
#include "../../common/physics/collider.hpp"
#include "../../common/physics/rigidBody.hpp"
#include <iostream>
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

    void HookSystem::generateBullet(Hook& hook)
    {
        auto* node = hook.getNode();
        auto& baseRg = node->getComponent<phys::RigidBody>();

        auto* bullet = node->createChild();
        auto& rg = bullet->addComponent<phys::RigidBody>(baseRg.getPosition(), 0.F, phys::BodyType::DYNAMIC);
        auto& col = bullet->addComponent<phys::Collider>(phys::Box{Vector2(), Vector2{0.3F, 0.3F}});
        col.start();
        col.setLayer("Bullet");

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
} // namespace stay
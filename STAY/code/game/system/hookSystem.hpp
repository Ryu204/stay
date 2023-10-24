#pragma once

#include <unordered_map>

#include "../../common/ecs/manager.hpp"

namespace stay
{
    namespace phys
    {
        class RigidBody;
    } // namespace phys

    struct Hook;

    struct HookSystem : public ecs::System, public ecs::UpdateSystem, public ecs::InputSystem
    {
            HookSystem(ecs::Manager* manager);
            void update(float dt) override;
            void input(const sf::Event& event) override;
        private:
            void generateBullet(Hook& hook);
            void clearBullet(Hook& hook);
            
            void queueForAttachment(Hook* hook, phys::RigidBody* obstacle);
            void updateCooldown(float dt);
            void updateDirection();
            void processQueue();
            
            Vector2 mDirection;
            std::unordered_map<Hook*, phys::RigidBody*> mQueued;
    };
} // namespace stay
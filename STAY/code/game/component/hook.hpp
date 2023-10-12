#pragma once

#include <unordered_set>

#include "../../common/ecs/manager.hpp"

namespace stay
{
    struct Hook : public ecs::Component
    {
        float speed{5.F};
        float cooldown{1.F};
        float timeToCD{0.F};
        Node* created{nullptr};
        bool shootable{true};
        SERIALIZE(speed, cooldown)

        void reset();
    };

    struct HookSystem : public ecs::System, public ecs::UpdateSystem, public ecs::InputSystem
    {
            HookSystem(ecs::Manager* manager);
            void update(float dt) override;
            void input(const sf::Event& event) override;
        private:
            void addToFixQueue(Hook& hook);
            void updateCooldown(float dt);
            void generateBullet(Hook& hook);
            void updateDirection();
            void fixQueued();

            Vector2 mDirection;
            std::unordered_set<Hook*> mQueueFixing;
    };
} // namespace stay
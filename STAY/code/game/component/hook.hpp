#pragma once

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
            void fixOnSurface(Hook& hook);
            void updateCooldown(float dt);
            void generateBullet(Hook& hook);
    };
} // namespace stay
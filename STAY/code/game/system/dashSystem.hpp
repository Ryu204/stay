#pragma once 

#include "../../common/ecs/manager.hpp"

namespace stay
{
    struct DashSystem : public ecs::InputSystem, public ecs::UpdateSystem, public ecs::System
    {
            DashSystem(ecs::Manager* manager);
            void update(float dt) override;
            void input(const sf::Event& event) override;
    };
} // namespace stay
#pragma once

#include "stay/ecs/component.hpp"

namespace stay
{
    struct Dash : public ecs::Component
    {
            float velocity{10.F};
            float length{4.F};
            float cooldown{3.F};
            float postBrake{0.5F};
            
            bool activated{false};
            bool canDash{true};
            bool left{false};
            
            COMPONENT(Dash, velocity, length, cooldown, postBrake);
    };
} // namespace stay
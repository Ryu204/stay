#pragma once

#include "../../common/ecs/component.hpp"

namespace stay
{
    struct Dash : public ecs::Component<Dash>
    {
            float velocity{10.F};
            float length{4.F};
            float cooldown{3.F};
            float postBrake{0.5F};
            
            bool activated{false};
            bool canDash{true};
            bool left{false};
            
            COMPONENT(dash, velocity, length, cooldown, postBrake);
    };
} // namespace stay
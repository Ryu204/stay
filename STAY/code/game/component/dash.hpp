#pragma once

#include "../../common/ecs/component.hpp"

namespace stay
{
    struct Dash : public ecs::Component
    {
            float velocity{10.F};
            float length{4.F};
            float cooldown{3.F};
            
            bool activated{false};
            bool canDash{true};
            bool left{false};
            
            SERIALIZE(velocity, length, cooldown);
    };
} // namespace stay
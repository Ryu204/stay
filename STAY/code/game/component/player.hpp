#pragma once

#include "../../common/physics/rigidBody.hpp"

namespace stay
{
    struct Player : public ecs::Component
    {
        // Base force to move with controls
        float moveStrength{15.F};
        float jumpHeight{5.F};
        // Set this to a higher-than-one float to create snappy movement
        float oppositeScale{1.F};
        // Same but apply to airbone moments
        float airScale{1.F};

        bool canJump{false};
        bool onGround{false};
        bool onRope{false};
        bool onDash{false};
        phys::RigidBody* movementBody;
        phys::RigidBody* hookBody;

        COMPONENT(Player, moveStrength, jumpHeight, oppositeScale, airScale);
    };
} // namespace stay
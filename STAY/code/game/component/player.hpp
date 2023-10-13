#pragma once

#include "../../common/physics/rigidBody.hpp"
#include "../../common/ecs/manager.hpp"

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
        phys::RigidBody* rgbody;
        SERIALIZE(moveStrength, jumpHeight, oppositeScale);
    };

    struct PlayerSystem 
        : public ecs::System
        , public ecs::StartSystem
        , public ecs::UpdateSystem
        , public ecs::InputSystem
    {
            PlayerSystem(ecs::Manager* manager);
            void start() override;
            void input(const sf::Event& event) override;
            void update(float dt) override;
        private:
            bool mEntered;
    };
} // namespace stay
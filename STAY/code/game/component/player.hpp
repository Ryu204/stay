#pragma once

#include "../../common/physics/rigidBody.hpp"
#include "../../common/ecs/manager.hpp"

namespace stay
{
    struct Player : public ecs::Component
    {
        float speed{5.F};
        float jumpHeight{5.F};
        float canJump{false};
        phys::RigidBody* rgbody;
        SERIALIZE(speed, jumpHeight)
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
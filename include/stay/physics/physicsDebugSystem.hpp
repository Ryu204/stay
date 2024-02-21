#pragma once

#include "stay/ecs/system.hpp"
#include "stay/physics/debugDraw.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    struct PhysicsDebugSystem 
        : public ecs::RenderSystem
        , public ecs::InitSystem
        , public ecs::InputSystem
        , public ecs::System
    {
            // REGISTER_SYSTEM(PhysicsDebugSystem);
            PhysicsDebugSystem(ecs::Manager* manager);
            ~PhysicsDebugSystem() override;
            void init(ecs::SystemContext& /* context */) override;
            void input(const sf::Event& event) override;
            void render(RTarget* target, Node* /*root*/) override;
        private:
            Uptr<phys::DebugDraw> mDrawer;
            bool mShouldDraw;
    };
} // namespace stay

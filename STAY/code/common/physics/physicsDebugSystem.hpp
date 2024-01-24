#pragma once

#include "ecs/system.hpp"
#include "physics/debugDraw.hpp"
#include "physics/world.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    namespace sys
    {
        struct PhysicsDebugSystem 
            : public ecs::RenderSystem
            , public ecs::InitSystem
            , public ecs::System
        {
                REGISTER_SYSTEM(PhysicsDebugSystem)
                PhysicsDebugSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(1)
                    , ecs::InitSystem(0)
                    , ecs::System(manager)
                { }
                ~PhysicsDebugSystem() override
                {
                    // Clear the drawer
                    if (phys::World::avail())
                    {
                        phys::World::get().SetDebugDraw(nullptr);
                    }
                }

                void init(ecs::SystemContext& /* context */) override
                {
                    mDrawer = std::make_unique<phys::DebugDraw>();
                    uint32 flags = 0;
                    flags += b2Draw::e_shapeBit;
                    // flags += b2Draw::e_jointBit;
                    // flags += b2Draw::e_aabbBit;
                    flags += b2Draw::e_centerOfMassBit;
                    mDrawer->SetFlags(flags);
                    phys::World::get().SetDebugDraw(mDrawer.get());
                }

                void render(RTarget* target, Node* /*root*/) override
                {
                    mDrawer->setRenderTarget(target);
                    phys::World::get().DebugDraw();
                }

            private:
                Uptr<phys::DebugDraw> mDrawer;
        };
    } // namespace sys
} // namespace stay

#pragma once

#include <box2d/box2d.h>

#include "../../common/ecs/manager.hpp"
#include "../../common/physics/debugDraw.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    namespace sys
    {
        struct PhysicsDebugSystem : public ecs::RenderSystem, public ecs::System
        {
                PhysicsDebugSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(1)
                    , ecs::System(manager)
                { }
                virtual ~PhysicsDebugSystem()
                {
                    // Clear the drawer
                    if (mPhysicsWorld != nullptr)
                    {
                        mPhysicsWorld->SetDebugDraw(nullptr);
                    }
                }

                void initialize(b2World* world)
                {
                    mDrawer = std::make_unique<phys::DebugDraw>();
                    mPhysicsWorld = world;
                    if (mPhysicsWorld != nullptr)
                    {
                        uint32 flags = 0;
                        flags += b2Draw::e_shapeBit;
                        flags += b2Draw::e_jointBit;
                        // flags += b2Draw::e_aabbBit;
                        flags += b2Draw::e_centerOfMassBit;
                        mDrawer->SetFlags(flags);
                        mPhysicsWorld->SetDebugDraw(mDrawer.get());
                    }
                }

                void render(RTarget* target, Node* /*root*/) override
                {
                    mDrawer->setRenderTarget(target);
                    mPhysicsWorld->DebugDraw();
                }

            private:
                b2World* mPhysicsWorld;
                Uptr<phys::DebugDraw> mDrawer;
        };
    } // namespace sys
} // namespace stay

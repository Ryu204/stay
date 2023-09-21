#pragma once

#include <box2d/box2d.h>

#include "../../common/ecs/manager.hpp"
#include "../../common/physics/debugDraw.hpp"

namespace stay
{
    namespace sys
    {
        struct RawPhysicsRenderSystem : public ecs::RenderSystem, public ecs::UpdateSystem, public ecs::System
        {
                RawPhysicsRenderSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(1)
                    , ecs::UpdateSystem(0)
                    , ecs::System(manager)
                { }
                virtual ~RawPhysicsRenderSystem()
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

                void render(RTarget* target) override
                {
                    mDrawer->setRenderTarget(target);
                    mPhysicsWorld->DebugDraw();
                }

                void update(float dt) override
                {
                    static const int velIterCount = 8;
                    static const int posIterCount = 3;
                    mPhysicsWorld->Step(dt, velIterCount, posIterCount);
                }

            private:
                b2World* mPhysicsWorld;
                Uptr<phys::DebugDraw> mDrawer;
        };
    } // namespace sys
} // namespace stay

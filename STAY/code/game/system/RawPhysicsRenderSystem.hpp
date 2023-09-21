#pragma once

#include <box2d/box2d.h>

#include "../../common/ecs/manager.hpp"
#include "../../common/physics/debugDraw.hpp"

namespace stay
{
    namespace sys
    {
        struct RawRenderSystem : public ecs::RenderSystem, public ecs::System
        {
                RawRenderSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(-1)
                    , ecs::System(manager)
                { }

                void initialize(RTarget* target, b2World* world)
                {
                    mDrawer = std::make_unique<phys::DebugDraw>(target);
                    mPhysicsWorld = world;
                    mPhysicsWorld->SetDebugDraw(mDrawer.get());
                }

                void render(RTarget* target) override
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

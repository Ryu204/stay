#pragma once

#include <box2d/box2d.h>

#include "../../common/ecs/manager.hpp"
#include "../../common/physics/rigidBody.hpp"
#include "../../common/world/node.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    namespace sys
    {
        struct PhysicsSystem 
            : public ecs::PreUpdateSystem
            , public ecs::UpdateSystem
            , public ecs::PostUpdateSystem
            , public ecs::System
        {
                PhysicsSystem(ecs::Manager* manager)
                    : ecs::PreUpdateSystem(0)
                    , ecs::UpdateSystem(-1)
                    , ecs::PostUpdateSystem(-1)
                    , ecs::System(manager)
                { }

                void initialize(b2World* world)
                {
                    mPhysicsWorld = world;
                }

                void preUpdate(float dt) override
                {
                }

                void update(float dt) override
                {
                    static const int velIterCount = 8;
                    static const int posIterCount = 3;
                    mPhysicsWorld->Step(dt, velIterCount, posIterCount);
                }

                // Bind transform to body collider
                void postUpdate(float dt) override
                {
                    auto view = mManager->getRegistry().view<phys::RigidBody>();

                    for (auto entity : view)
                    {
                        auto* node = Node::getNode(entity);
                        auto& rgbody = view.get<phys::RigidBody>(entity);
                        // Unoptimized chunk of code
                        auto newTf = Transform(rgbody.getPosition(), rgbody.getAngle());
                        node->setGlobalTransform(newTf);
                        // End unoptimized chunk of code
                    }
                }
            private:
                b2World* mPhysicsWorld;
        };
    } // namespace sys
} // namespace stay

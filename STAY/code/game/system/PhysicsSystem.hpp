#pragma once

#include <box2d/box2d.h>

#include "../../common/physics/rigidBody.hpp"
#include "../../common/physics/collider.hpp"
#include "../../common/ecs/manager.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    namespace sys
    {
        namespace detail
        {
            class ContactListener : public b2ContactListener
            {
                void BeginContact(b2Contact* contact) override
                { 
                    auto* A = phys::Collider::getCollider(contact->GetFixtureA());
                    auto* B = phys::Collider::getCollider(contact->GetFixtureB());
                    A->OnCollisionEnter.invoke(*B, *contact);
                    B->OnCollisionEnter.invoke(*A, *contact);
                }
                void EndContact(b2Contact* contact) 
                { 
                    auto* A = phys::Collider::getCollider(contact->GetFixtureA());
                    auto* B = phys::Collider::getCollider(contact->GetFixtureB());
                    A->OnCollisionExit.invoke(*B, *contact);
                    B->OnCollisionExit.invoke(*A, *contact);
                }
            };
        } // namespace detail

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

                virtual ~PhysicsSystem()
                {
                    mPhysicsWorld->SetContactListener(nullptr);
                }

                void initialize(b2World* world)
                {
                    mPhysicsWorld = world;
                    world->SetContactListener(&mContactListener);
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
                detail::ContactListener mContactListener;
        };
    } // namespace sys
} // namespace stay

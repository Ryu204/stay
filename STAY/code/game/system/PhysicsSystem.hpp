#pragma once

#include <box2d/box2d.h>

#include "../../common/physics/rigidBody.hpp"
#include "../../common/physics/collider.hpp"
#include "../../common/ecs/manager.hpp"

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
                    auto* aCol = reinterpret_cast<phys::Collider*>(contact->GetFixtureA()->GetUserData().pointer);
                    auto* bCol = reinterpret_cast<phys::Collider*>(contact->GetFixtureB()->GetUserData().pointer);
                    bool isTrigger = aCol->getTrigger() || bCol->getTrigger();
                    if (!isTrigger)
                    {
                        aCol->OnCollisionEnter.invoke(*bCol, *contact);
                        bCol->OnCollisionEnter.invoke(*aCol, *contact);
                    }
                    else
                    {
                        aCol->OnTriggerEnter.invoke(*bCol, *contact);
                        bCol->OnTriggerEnter.invoke(*aCol, *contact);
                    }
                }
                void EndContact(b2Contact* contact) 
                { 
                   auto* aCol = reinterpret_cast<phys::Collider*>(contact->GetFixtureA()->GetUserData().pointer);
                    auto* bCol = reinterpret_cast<phys::Collider*>(contact->GetFixtureB()->GetUserData().pointer);
                    bool isTrigger = aCol->getTrigger() || bCol->getTrigger();
                    if (!isTrigger)
                    {
                        aCol->OnCollisionExit.invoke(*bCol, *contact);
                        bCol->OnCollisionExit.invoke(*aCol, *contact);
                    }
                    else
                    {
                        aCol->OnTriggerExit.invoke(*bCol, *contact);
                        bCol->OnTriggerExit.invoke(*aCol, *contact);
                    }
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

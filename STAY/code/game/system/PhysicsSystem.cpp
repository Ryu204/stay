#include "PhysicsSystem.hpp"
#include "../../common/physics/world.hpp"

namespace stay
{
    namespace sys
    {
        namespace detail
        {
            void ContactListener::BeginContact(b2Contact* contact)
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
            void ContactListener::EndContact(b2Contact* contact) 
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
        } // namespace detail

        PhysicsSystem::PhysicsSystem(ecs::Manager* manager)
            : ecs::StartSystem(0)
            , ecs::UpdateSystem(-1)
            , ecs::System(manager)
        { 
            phys::Collider::mCollisionLayer()
                .set(0, "Default")
                .set(1, "Player")
                .set(2, "Bullet")
                .set("Player", "Bullet", false);
        }

        PhysicsSystem::~PhysicsSystem()
        {
            if (phys::World::avail())
            {
                phys::World::get().SetContactListener(nullptr);
            }
        }

        void PhysicsSystem::start()
        {
            auto view2 = mManager->getRegistryRef().view<phys::Collider>();
            for (auto entity : view2)
            {
                view2.get<phys::Collider>(entity).start();
            }
        }

        void PhysicsSystem::initialize()
        {
            phys::World::get().SetContactListener(&mContactListener);
        }

        void PhysicsSystem::update(float dt)
        {
            static const int velIterCount = 8;
            static const int posIterCount = 3;
            phys::World::get().Step(dt, velIterCount, posIterCount);
            
            auto view = mManager->getRegistryRef().view<phys::RigidBody>();
            mBatched.clear();
            for (auto entity : view)
                batchSingle(entity);
        }

        void PhysicsSystem::batchSingle(ecs::Entity entity)
        {
            if (mBatched.count(entity) > 0)
                return;
            if (!mManager->hasComponent<phys::RigidBody>(entity))
            {
                mBatched.insert(entity);
                return;
            }
            const auto& rgbody = mManager->getComponent<phys::RigidBody>(entity);
            Transform tf;
            tf.setPosition(rgbody.getPosition());
            tf.setRotation(rgbody.getAngle());
            auto* node = Node::getNode(entity);

            if (node->stray())
            {
                node->setLocalTransform(tf);
                mBatched.insert(entity);
                return;
            }
            bool parentIsBatched = mBatched.count(node->parent()->entity()) > 0;
            if (!parentIsBatched)
                batchSingle(node->parent()->entity());
            node->setGlobalTransform(tf);
            mBatched.insert(entity);
        }
    } // namespace sys
} // namespace stay

#include "PhysicsSystem.hpp"
#include "physics/rigidBody.hpp"
#include "physics/collider.hpp"
#include "physics/joint.hpp"
#include "physics/world.hpp"
#include "utility/math.hpp"

namespace stay
{
    namespace sys
    {
        namespace detail
        {
            void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* /*manifold*/) 
            {
                phys::Collision aInfo(contact, /*isA=*/true);
                phys::Collision bInfo(contact, /*isA=*/false);
                auto* aCol = bInfo.other;
                auto* bCol = aInfo.other;
                aCol->OnCollisionDetection.invoke(aInfo, *contact);
                bCol->OnCollisionDetection.invoke(bInfo, *contact);
            }

            void ContactListener::BeginContact(b2Contact* contact)
            { 
                phys::Collision aInfo(contact, /*isA=*/true);
                phys::Collision bInfo(contact, /*isA=*/false);
                auto* aCol = bInfo.other;
                auto* bCol = aInfo.other;
                const bool isTrigger = aCol->getTrigger() || bCol->getTrigger();
                if (!isTrigger)
                {
                    aCol->OnCollisionEnter.invoke(aInfo);
                    bCol->OnCollisionEnter.invoke(bInfo);
                }
                else
                {
                    aCol->OnTriggerEnter.invoke(aInfo);
                    bCol->OnTriggerEnter.invoke(bInfo);
                }
            }
            void ContactListener::EndContact(b2Contact* contact) 
            { 
                phys::Collision aInfo(contact, /*isA=*/true);
                phys::Collision bInfo(contact, /*isA=*/false);
                auto* aCol = bInfo.other;
                auto* bCol = aInfo.other;
                const bool isTrigger = aCol->getTrigger() || bCol->getTrigger();
                if (!isTrigger)
                {
                    aCol->OnCollisionExit.invoke(aInfo);
                    bCol->OnCollisionExit.invoke(bInfo);
                }
                else
                {
                    aCol->OnTriggerExit.invoke(aInfo);
                    bCol->OnTriggerExit.invoke(bInfo);
                }
            }
        } // namespace detail

        PhysicsSystem::PhysicsSystem(ecs::Manager* manager)
            : ecs::InitSystem(0)
            , ecs::StartSystem(0)
            , ecs::UpdateSystem(-1)
            , ecs::System<PhysicsSystem>(manager)
        { 
            phys::Collider::mCollisionLayer()
                .set(0, "Default")
                .set(1, "Player")
                .set(2, "Bullet")
                .set(3, "Isolate")
                .set("Player", "Bullet", false)
                .set("Player", "Player", false)
                .isolate("Isolate");
        }

        PhysicsSystem::~PhysicsSystem()
        {
            if (phys::World::avail())
            {
                phys::World::get().SetContactListener(nullptr);
                phys::World::get().SetDestructionListener(nullptr);
            }
        }

        void PhysicsSystem::start()
        {
            auto colView = mManager->getRegistryRef().view<phys::Collider>();
            for (auto entity : colView)
            {
                colView.get<phys::Collider>(entity).connect();
            }
            auto jointView = mManager->getRegistryRef().view<phys::Joint>();
            for (auto [entity, joint] : jointView.each())
            {
                joint.start();
            }
        }

        void PhysicsSystem::init(ecs::SystemContext& /* context */)
        {
            phys::World::get().SetContactListener(&mContactListener);
            phys::World::get().SetDestructionListener(&mDestructListener);
        }

        void PhysicsSystem::update(float dt)
        {
            applyHorizontalDamping(dt);

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
            const bool parentIsBatched = mBatched.count(node->parent()->entity()) > 0;
            if (!parentIsBatched)
                batchSingle(node->parent()->entity());
            node->setGlobalTransform(tf);
            mBatched.insert(entity);
        }

        void PhysicsSystem::applyHorizontalDamping(float dt)
        {
            auto view = mManager->getRegistryRef().view<phys::RigidBody>();
            for (auto [entity, rgbody] : view.each())
            {
                const float lostRatio = 1 - utils::clamp01(rgbody.horizontalDamping());
                const auto vel = rgbody.getVelocity();
                rgbody.setVelocity(vel * std::pow(lostRatio, dt));
            }
        }
    } // namespace sys
} // namespace stay

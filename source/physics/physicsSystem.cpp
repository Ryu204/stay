#include "stay/physics/physicsSystem.hpp"
#include "stay/physics/rigidBody.hpp"
#include "stay/physics/collider.hpp"
#include "stay/physics/joint.hpp"
#include "stay/physics/world.hpp"
#include "stay/utility/math.hpp"

namespace stay
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
        : ecs::InitSystem{0}
        , ecs::StartSystem{0}
        , ecs::UpdateSystem{-2}
        , ecs::ConfigurableSystem{0}
        , ecs::System(manager)
    {}

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

    bool PhysicsSystem::loadConfig(const Serializable::Data& data)
    {
        const auto valid = data.contains("layers") && data.contains("isolate") && data.contains("disable")
            && data["layers"].is_array() && data["isolate"].is_array() && data["disable"].is_object();
        if (!valid)
            return false;
        auto& collisionSettings = phys::Collider::collisionLayers();
        for (auto i = 0; i < data["layers"].size(); ++i)
        {
            for (auto j = i; j < data["layers"].size(); ++j)
                collisionSettings.set(i, j, true);
            collisionSettings.set(i, data["layers"][i].get<std::string>());
        }
        for (const auto& i : data["isolate"])
            collisionSettings.isolate(i.get<std::string>());
        for (const auto& i : data["disable"].items())
        {
            if (!i.value().is_array())
                return false;
            for (const auto& j : i.value())
                collisionSettings.set(i.key(), j.get<std::string>(), false);
        }
        return true;
    }

    void PhysicsSystem::batchSingle(ecs::Entity entity)
    {
        if (mBatched.count(entity) > 0) 
        {
            return;
        }
        auto* node = Node::getNode(entity);
        if (!node->stray() && mBatched.count(node->parent()->entity()) == 0) 
        {
            batchSingle(node->parent()->entity());
        }
        if (!node->hasComponent<phys::RigidBody>())
        {
            mBatched.insert(entity);
            return;
        }
        const auto& rgbody = mManager->getComponent<phys::RigidBody>(entity);
        Transform tf{rgbody.getPosition(), rgbody.getAngle()};
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
} // namespace stay

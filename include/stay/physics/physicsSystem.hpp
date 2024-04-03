#pragma once

#include <box2d/b2_contact.h>
#include <unordered_set>

#include <box2d/box2d.h>

#include "stay/physics/destructRegister.hpp"
#include "stay/ecs/system.hpp"

namespace stay
{
    namespace detail
    {
        class ContactListener : public b2ContactListener
        {
            void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
            void BeginContact(b2Contact* contact) override;
            void EndContact(b2Contact* contact) override;
        };
    } // namespace detail

    struct PhysicsSystem 
        : public ecs::InitSystem
        , public ecs::StartSystem
        , public ecs::UpdateSystem
        , public ecs::ConfigurableSystem
        , public ecs::System
    {
            // REGISTER_SYSTEM(PhysicsSystem)
            PhysicsSystem(ecs::Manager* manager);
            ~PhysicsSystem() override;
            void start() override;
            void init(ecs::SystemContext& /*context*/) override;
            void update(float dt) override;
            bool loadConfig(const Serializable::Data& data) override;
        private:
            void batchSingle(ecs::Entity entity);
            void applyHorizontalDamping(float dt);

            detail::ContactListener mContactListener;
            phys::DestructRegister mDestructListener;
            std::unordered_set<ecs::Entity> mBatched;
            Vector2 mGravity{};
    };
} // namespace stay

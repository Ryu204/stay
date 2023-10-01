#pragma once

#include <unordered_set>

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
                void BeginContact(b2Contact* contact) override;
                void EndContact(b2Contact* contact) override;
            };
        } // namespace detail

        struct PhysicsSystem 
            : public ecs::StartSystem
            , public ecs::PreUpdateSystem
            , public ecs::UpdateSystem
            , public ecs::PostUpdateSystem
            , public ecs::System
        {
                PhysicsSystem(ecs::Manager* manager);
                virtual ~PhysicsSystem();
                void start() override;
                void initialize(b2World* world);
                void preUpdate(float dt) override;
                void update(float dt) override;
                // Bind transform to body collider
                void postUpdate(float /*dt*/) override;
            private:
                void batchToPhysicsPos(ecs::Entity entity);

                b2World* mPhysicsWorld;
                detail::ContactListener mContactListener;
                std::unordered_set<ecs::Entity> mBatched;
        };
    } // namespace sys
} // namespace stay

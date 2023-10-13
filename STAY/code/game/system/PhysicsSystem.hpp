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
            , public ecs::UpdateSystem
            , public ecs::System
        {
                PhysicsSystem(ecs::Manager* manager);
                virtual ~PhysicsSystem();
                void start() override;
                void initialize();
                void update(float dt) override;
            private:
                void batchSingle(ecs::Entity entity);
                void applyHorizontalDamping(float dt);

                detail::ContactListener mContactListener;
                std::unordered_set<ecs::Entity> mBatched;
        };
    } // namespace sys
} // namespace stay

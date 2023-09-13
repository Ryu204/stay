#pragma once

#include <array>

#include <entt/entity/registry.hpp>

namespace stay
{
    namespace ecs
    {
        using Entity = entt::entity;
        using Registry = entt::registry;
        class Manager;
        // Base class for convinience when reference manager
        struct System
        {
            System(Manager* manager)
                : mManager(manager)
            {}
            virtual ~System() = default;
            
            protected:
                Manager* mManager;
        };
        // Interfaces for specific tasks
        struct StartSystem
        {
            StartSystem(int order)
                : orderStart(order)
            {}
            virtual ~StartSystem() = default;
            int orderStart;
            virtual void start() = 0;
        };
        struct UpdateSystem
        {
            UpdateSystem(int order)
                : orderUpdate(order)
            {}
            virtual ~UpdateSystem() = default;
            int orderUpdate;
            virtual void update(float/*dt*/) = 0;
        };
    }
} // namespace stay

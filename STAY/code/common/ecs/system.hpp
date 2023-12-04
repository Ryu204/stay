#pragma once

#include "../utility/typedef.hpp"
#include "type.hpp"

namespace stay
{
    class Node;
    namespace ecs
    {
        class Manager;
        // Base class for convinience when reference manager
        struct System
        {
                System(Manager* manager);
                virtual ~System() = default;
                Registry& registry();            
            protected:
                Manager* mManager;
        };
        // Interfaces for specific tasks
        struct StartSystem
        {
            StartSystem(int order);
            virtual ~StartSystem() = default;
            int orderStart;
            virtual void start() = 0;
        };
        struct UpdateSystem
        {
            UpdateSystem(int order);
            virtual ~UpdateSystem() = default;
            int orderUpdate;
            virtual void update(float/*dt*/) = 0;
        };
        struct PostUpdateSystem
        {
            PostUpdateSystem(int order);
            virtual ~PostUpdateSystem() = default;
            int orderLateUpdate;
            virtual void postUpdate(float/*dt*/) = 0;
        };
        struct PreUpdateSystem
        {
            PreUpdateSystem(int order);
            virtual ~PreUpdateSystem() = default;
            int orderPreUpdate;
            virtual void preUpdate(float/*dt*/) = 0;
        };
        struct RenderSystem
        {
            RenderSystem(int order);
            virtual ~RenderSystem() = default;
            int orderRender;
            virtual void render(RTarget* target, Node* root) = 0;
        };
        struct InputSystem
        {
            InputSystem(int order);
            virtual ~InputSystem() = default;
            int orderInput;
            virtual void input(const sf::Event&/*event*/) = 0;
        };
    }
} // namespace stay

#pragma once

#include "systemContext.hpp"
#include "stay/type/serializable.hpp"

namespace stay
{
    class Node;
    namespace ecs
    {
        // Interfaces for specific tasks
        struct InitSystem
        {
            InitSystem(int order);
            virtual ~InitSystem() = default;
            int orderInit;
            virtual void init(SystemContext& context) = 0;
        };
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
        struct ConfigurableSystem
        {
            ConfigurableSystem(int order);
            int orderConfig;
            virtual bool loadConfig(const Serializable::Data& data) = 0;
        };
    }
} // namespace stay

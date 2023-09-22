#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "system.hpp"
#include "../utility/typedef.hpp"
#include "../utility/typedef.hpp"
#include "../world/node.hpp"

namespace stay
{
    class Component;
    namespace ecs
    {        
        namespace detail
        {
            template <typename T>
            struct Ordered
            {
                int id {};
                T val {};
            };
            template <typename T>
            struct Cmpr
            {
                bool operator () (const Ordered<T>& sml, const Ordered<T>& lgr)
                {
                    return sml.id < lgr.id;
                }
            };
        } // namespace detail
        class Manager 
        {
            public:                
                Registry& getRegistry();
                // @param DerivedSystem A type is used as `DerivedSystem` if it's a `RenderSystem`, `UpdateSystem`,.etc (see `system.hpp`)
                // @note Its overriding IDs are used to determine its order in the manager action, i.e type with lower ID called earlier
                template <typename DerviedSystem>
                SPtr<DerviedSystem> registerSystem();
                // Meant to be called only once, before any update
                void start();
                // Meant to be call every frame update
                void update(float dt);
                void render(RTarget* target);
                void input(const sf::Event& event);

                // Generate a new node
                Node* create(Node* parent = &Node::root());
                // Components related functions
                template <typename Type, typename... Args, whereIs(Type, Component)>
                Type& addComponents(Node* node, Args&&... args);
                template <typename Type, whereIs(Type, Component)>
                void removeComponents(Node* node);
                template <typename Type, whereIs(Type, Component)>
                bool hasComponent(const Node* node) const;
                template <typename Type, whereIs(Type, Component)>
                Type& getComponent(Node* node);
            private:
                template <typename T>
                using Pair = detail::Ordered<SPtr<T>>;
                Registry mRegistry{};
                std::vector<Pair<StartSystem>> mStartSystems{};
                std::vector<Pair<UpdateSystem>> mUpdateSystems{};
                std::vector<Pair<PostUpdateSystem>> mPostUpdateSystems{};
                std::vector<Pair<PreUpdateSystem>> mPreUpdateSystems{};
                std::vector<Pair<RenderSystem>> mRenderSystems{};
                std::vector<Pair<InputSystem>> mInputSystems{};
        };

        template <typename Type, typename... Args, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        Type& Manager::addComponents(Node* node, Args&&... args)
        {
            auto entity = node->getEntity();
            auto& res = mRegistry.emplace<Type>(entity, std::forward<Args>(args)...);
            // assign this entity to the component so it knows which node does it belong to
            res.assign(entity);
            return res;
        }
        template <typename Type, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        void Manager::removeComponents(Node* node)
        {
            mRegistry.remove<Type>(node->getEntity());
        }
        template <typename Type, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        bool Manager::hasComponent(const Node* node) const
        {
            return mRegistry.try_get<Type>(node->getEntity()) != nullptr;
        }
        template <typename Type, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        Type& Manager::getComponent(Node* node)
        {
            return mRegistry.get<Type>(node->getEntity());
        }

        template <typename DerviedSystem>
        SPtr<DerviedSystem> Manager::registerSystem()
        {
            SPtr<DerviedSystem> ptr = std::make_shared<DerviedSystem>(this);
            // Start
            SPtr<StartSystem> startPtr = std::dynamic_pointer_cast<StartSystem>(ptr);
            if (startPtr.get() != nullptr)
                mStartSystems.push_back(Pair<StartSystem>{ startPtr->orderStart, startPtr });
            // Update
            SPtr<UpdateSystem> updatePtr = std::dynamic_pointer_cast<UpdateSystem>(ptr);
            if (updatePtr.get() != nullptr)
                mUpdateSystems.push_back(Pair<UpdateSystem>{updatePtr->orderUpdate, updatePtr});
            // PostUpdate
            SPtr<PostUpdateSystem> lateUpdatePtr = std::dynamic_pointer_cast<PostUpdateSystem>(ptr);
            if (lateUpdatePtr.get() != nullptr)
                mPostUpdateSystems.push_back(Pair<PostUpdateSystem>{lateUpdatePtr->orderLateUpdate, lateUpdatePtr});
            // PreUpdate
            SPtr<PreUpdateSystem> preUpdatePtr = std::dynamic_pointer_cast<PreUpdateSystem>(ptr);
            if (preUpdatePtr.get() != nullptr)
                mPreUpdateSystems.push_back(Pair<PreUpdateSystem>{preUpdatePtr->orderPreUpdate, preUpdatePtr});
            // Render
            SPtr<RenderSystem> renderPtr = std::dynamic_pointer_cast<RenderSystem>(ptr);
            if (renderPtr.get() != nullptr)
                mRenderSystems.push_back(Pair<RenderSystem>{renderPtr->orderRender, renderPtr});
            // Input
            SPtr<InputSystem> inputPtr = std::dynamic_pointer_cast<InputSystem>(ptr);
            if (inputPtr.get() != nullptr)
                mInputSystems.push_back(Pair<InputSystem>{inputPtr->orderInput, inputPtr});

            return ptr;
        }
    } // namespace ecs
} // namespace stay
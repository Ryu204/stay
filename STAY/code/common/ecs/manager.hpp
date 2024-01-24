#pragma once

#include "system.hpp"
#include "utility/typedef.hpp"
#include "world/node.hpp"

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
                Registry& getRegistryRef();
                SPtr<Registry> getRegistry();

                // @note Its overriding IDs are used to determine its order in the manager action, i.e type with lower ID called earlier
                // @param DerivedSystem A dervied class of `ecs:*System`(s) defined in `system.hpp`, with the restriction of having a constructor taking an `ecs::Manager*`
                template <typename DerviedSystem>
                SPtr<DerviedSystem> registerSystem();
                // Meant to be called only once, before any update
                void start();
                // Meant to be call every frame update
                void update(float dt);
                void render(RTarget* target, Node* root);
                void input(const sf::Event& event);

                // Components related functions
                template <typename Type, typename... Args, whereIs(Type, Component)>
                Type& addComponent(Node* node, Args&&... args)
                {
                    return addComponent<Type, Args...>(node->entity(), std::forward<Args>(args)...);
                }
                template <typename Type, typename... Args, whereIs(Type, Component)>
                Type& addComponent(Entity entity, Args&&... args);
                template <typename Type, whereIs(Type, Component)>
                void removeComponents(Node* node)
                {
                    removeComponents<Type>(node->entity());
                }
                template <typename Type, whereIs(Type, Component)>
                void removeComponents(Entity entity);
                template <typename Type, whereIs(Type, Component)>
                bool hasComponent(const Node* node) const
                {
                    return hasComponent<Type>(node->entity());
                }
                template <typename Type, whereIs(Type, Component)>
                bool hasComponent(Entity entity) const;
                template <typename Type, whereIs(Type, Component)>
                Type& getComponent(Node* node)
                {
                    return getComponent<Type>(node->entity());
                }
                template <typename Type, whereIs(Type, Component)>
                Type& getComponent(Entity entity);
            private:
                template <typename T>
                using Pair = detail::Ordered<SPtr<T>>;
                SPtr<Registry> mRegistry{std::make_shared<Registry>()};
                std::vector<Pair<StartSystem>> mStartSystems{};
                std::vector<Pair<UpdateSystem>> mUpdateSystems{};
                std::vector<Pair<PostUpdateSystem>> mPostUpdateSystems{};
                std::vector<Pair<PreUpdateSystem>> mPreUpdateSystems{};
                std::vector<Pair<RenderSystem>> mRenderSystems{};
                std::vector<Pair<InputSystem>> mInputSystems{};
        };

        template <typename Type, typename... Args, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        Type& Manager::addComponent(Entity entity, Args&&... args)
        {
            auto& res = mRegistry->emplace<Type>(entity, std::forward<Args>(args)...);
            // assign this entity to the component so it knows which node does it belong to
            res.assign(entity);
            return res;
        }
        template <typename Type, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        void Manager::removeComponents(Entity entity)
        {
            mRegistry->remove<Type>(entity);
        }
        template <typename Type, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        bool Manager::hasComponent(Entity entity) const
        {
            return mRegistry->try_get<Type>(entity) != nullptr;
        }
        template <typename Type, std::enable_if_t<std::is_base_of_v<Component, Type>, bool>>
        Type& Manager::getComponent(Entity entity)
        {
            return mRegistry->get<Type>(entity);
        }

        template <typename DerviedSystem>
        SPtr<DerviedSystem> Manager::registerSystem()
        {
            SPtr<DerviedSystem> ptr = std::make_shared<DerviedSystem>(this);
            // Start
            if constexpr (std::is_base_of_v<StartSystem, DerviedSystem>)
            {
                SPtr<StartSystem> startPtr = std::dynamic_pointer_cast<StartSystem>(ptr);
                mStartSystems.push_back(Pair<StartSystem>{ startPtr->orderStart, startPtr });
            }
            // Update
            if constexpr (std::is_base_of_v<UpdateSystem, DerviedSystem>)
            {
                SPtr<UpdateSystem> updatePtr = std::dynamic_pointer_cast<UpdateSystem>(ptr);
                mUpdateSystems.push_back(Pair<UpdateSystem>{updatePtr->orderUpdate, updatePtr});
            }
            // PostUpdate
            if constexpr (std::is_base_of_v<PostUpdateSystem, DerviedSystem>)
            {
                SPtr<PostUpdateSystem> lateUpdatePtr = std::dynamic_pointer_cast<PostUpdateSystem>(ptr);
                mPostUpdateSystems.push_back(Pair<PostUpdateSystem>{lateUpdatePtr->orderLateUpdate, lateUpdatePtr});
            }
            // PreUpdate
            if constexpr (std::is_base_of_v<PreUpdateSystem, DerviedSystem>)
            {
                SPtr<PreUpdateSystem> preUpdatePtr = std::dynamic_pointer_cast<PreUpdateSystem>(ptr);
                mPreUpdateSystems.push_back(Pair<PreUpdateSystem>{preUpdatePtr->orderPreUpdate, preUpdatePtr});
            }
            // Render
            if constexpr (std::is_base_of_v<RenderSystem, DerviedSystem>)
            {
                SPtr<RenderSystem> renderPtr = std::dynamic_pointer_cast<RenderSystem>(ptr);
                mRenderSystems.push_back(Pair<RenderSystem>{renderPtr->orderRender, renderPtr});
            }
            // Input
            if constexpr (std::is_base_of_v<InputSystem, DerviedSystem>)
            {
                SPtr<InputSystem> inputPtr = std::dynamic_pointer_cast<InputSystem>(ptr);
                mInputSystems.push_back(Pair<InputSystem>{inputPtr->orderInput, inputPtr});
            }

            return ptr;
        }
    } // namespace ecs
} // namespace stay
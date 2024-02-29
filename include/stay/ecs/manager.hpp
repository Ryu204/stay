#pragma once

#include "systemTypes.hpp"
#include "stay/utility/typedef.hpp"
#include "stay/world/node.hpp"
#include "stay/program/builtinSystems.hpp"

namespace stay
{
    class Component;
    namespace ecs
    {        
        namespace detail
        {
            template <typename T>
            struct SystemWrapper
            {
                SystemWrapper(int updateOrder, SPtr<T> val, std::string name)
                    : updateOrder{updateOrder}
                    , val{val}
                    , name{std::move(name)}
                {}
                int updateOrder {};
                SPtr<T> val {};
                std::string name{};
            };
            template <typename T>
            struct Cmpr
            {
                bool operator () (const SystemWrapper<T>& sml, const SystemWrapper<T>& lgr)
                {
                    return sml.updateOrder < lgr.updateOrder;
                }
            };
        } // namespace detail

        class Manager 
        {
            public:     
                Manager() = default;
                Registry& getRegistryRef();
                SPtr<Registry> getRegistry();

                template <typename DerviedSystem>
                void registerSystem(const std::string& systemName = "unamed");
                void reset(ecs::SystemContext context, const Serializable::Data& systemConfigs, int systems = systems::Type::All);
                void start();
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
                using Wrapper = detail::SystemWrapper<T>;
                SPtr<Registry> mRegistry{std::make_shared<Registry>()};
                std::vector<Wrapper<InitSystem>> mInitSystems{};
                std::vector<Wrapper<StartSystem>> mStartSystems{};
                std::vector<Wrapper<UpdateSystem>> mUpdateSystems{};
                std::vector<Wrapper<PostUpdateSystem>> mPostUpdateSystems{};
                std::vector<Wrapper<PreUpdateSystem>> mPreUpdateSystems{};
                std::vector<Wrapper<RenderSystem>> mRenderSystems{};
                std::vector<Wrapper<InputSystem>> mInputSystems{};
                std::vector<Wrapper<ConfigurableSystem>> mConfigurableSystems{};
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

        template <typename DerivedSystem>
        void Manager::registerSystem(const std::string& systemName)
        {
            SPtr<DerivedSystem> ptr = std::make_shared<DerivedSystem>(this);
            // Init
            if constexpr (std::is_base_of_v<InitSystem, DerivedSystem>)
            {
                SPtr<InitSystem> initPtr = std::dynamic_pointer_cast<InitSystem>(ptr);
                mInitSystems.emplace_back(initPtr->orderInit, initPtr, systemName);
            }
            // Start
            if constexpr (std::is_base_of_v<StartSystem, DerivedSystem>)
            {
                SPtr<StartSystem> startPtr = std::dynamic_pointer_cast<StartSystem>(ptr);
                mStartSystems.emplace_back(startPtr->orderStart, startPtr, systemName);
            }
            // Update
            if constexpr (std::is_base_of_v<UpdateSystem, DerivedSystem>)
            {
                SPtr<UpdateSystem> updatePtr = std::dynamic_pointer_cast<UpdateSystem>(ptr);
                mUpdateSystems.emplace_back(updatePtr->orderUpdate, updatePtr, systemName);
            }
            // PostUpdate
            if constexpr (std::is_base_of_v<PostUpdateSystem, DerivedSystem>)
            {
                SPtr<PostUpdateSystem> lateUpdatePtr = std::dynamic_pointer_cast<PostUpdateSystem>(ptr);
                mPostUpdateSystems.emplace_back(lateUpdatePtr->orderLateUpdate, lateUpdatePtr, systemName);
            }
            // PreUpdate
            if constexpr (std::is_base_of_v<PreUpdateSystem, DerivedSystem>)
            {
                SPtr<PreUpdateSystem> preUpdatePtr = std::dynamic_pointer_cast<PreUpdateSystem>(ptr);
                mPreUpdateSystems.emplace_back(preUpdatePtr->orderPreUpdate, preUpdatePtr, systemName);
            }
            // Render
            if constexpr (std::is_base_of_v<RenderSystem, DerivedSystem>)
            {
                SPtr<RenderSystem> renderPtr = std::dynamic_pointer_cast<RenderSystem>(ptr);
                mRenderSystems.emplace_back(renderPtr->orderRender, renderPtr, systemName);
            }
            // Input
            if constexpr (std::is_base_of_v<InputSystem, DerivedSystem>)
            {
                SPtr<InputSystem> inputPtr = std::dynamic_pointer_cast<InputSystem>(ptr);
                mInputSystems.emplace_back(inputPtr->orderInput, inputPtr, systemName);
            }
            // Config
            if constexpr (std::is_base_of_v<ConfigurableSystem, DerivedSystem>)
            {
                SPtr<ConfigurableSystem> configPtr = std::dynamic_pointer_cast<ConfigurableSystem>(ptr);
                mConfigurableSystems.emplace_back(configPtr->orderConfig, configPtr, systemName);
            }
        }

        Manager& manager();
    } // namespace ecs
} // namespace stay
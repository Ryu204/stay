#pragma once

#include "componentLoader.hpp"
#include "manager.hpp"

namespace stay
{
    namespace ecs
    {
        namespace detail
        {
            template <typename T>
            bool ComponentSerializer<T>::deserialize(ecs::Manager& manager, ecs::Entity entity, const Serializable::Data& val) const
            {
                auto& comp = manager.addComponent<T>(entity);
                return comp.deserialization(val);
            }
            template <typename T>
            Serializable::Data ComponentSerializer<T>::serialize(ecs::Manager& manager, ecs::Entity entity) const
            {
                return manager.getComponent<T>(entity).serialize();
            }
            template <typename T>
            bool ComponentSerializer<T>::inEntity(ecs::Manager& manager, ecs::Entity entity) const
            {
                return manager.hasComponent<T>(entity);
            }
            template <typename T, std::enable_if_t<std::is_base_of_v<Component, T>, bool>>
            void ComponentsLoader::registerComponent(const std::string& name)
            {
                assert(mLoaderList.find(name) == mLoaderList.end() && "component name registered twice");
                mLoaderList.emplace(name, std::make_unique<ComponentSerializer<T>>());                
            }
        } // namespace detail
    } // namespace ecs
} // namespace stay
#pragma once

#include "ecs/type.hpp"
#include "type/serializable.hpp"

namespace stay
{
    namespace ecs
    {
        class Manager;
        class Component;
        namespace detail
        {
            class IComponentSerializer 
            {
                public:
                    virtual ~IComponentSerializer() = default;
                    virtual bool deserialize(ecs::Manager& /*manager*/, ecs::Entity /*entity*/, const Serializable::Data& /*val*/) const = 0;
                    virtual Serializable::Data serialize(ecs::Manager& /*manager*/, ecs::Entity /*entity*/) const = 0;
                    virtual bool inEntity(ecs::Manager& /* manager */, ecs::Entity /* entity */) const = 0;
            };

            template <typename T>
            class ComponentSerializer : public IComponentSerializer
            {
                public:
                    bool deserialize(ecs::Manager& manager, ecs::Entity entity, const Serializable::Data& val) const override;
                    Serializable::Data serialize(ecs::Manager& manager, ecs::Entity entity) const override;
                    bool inEntity(ecs::Manager& manager, ecs::Entity entity) const override;
            };

            // This class throws like a maniac, use with appropriate `try/catch` when loading components
            class ComponentsLoader
            {
                public:
                    template <typename T, whereIs(T, ecs::Component)>
                    void registerComponent(const std::string& name);

                    void loadAllComponents(ecs::Manager& manager, ecs::Entity entity, const Serializable::Data& componentsArray);
                    void loadComponent(ecs::Manager& manager, ecs::Entity entity, const std::string& name, const Serializable::Data& componentData);
                    Serializable::Data saveAllComponents(ecs::Manager& manager, ecs::Entity entity) const;
                    Serializable::Data saveComponent(ecs::Manager& manager, ecs::Entity entity, const std::string& name) const;
                private:
                    std::unordered_map<std::string, Uptr<IComponentSerializer>> mLoaderList;
            };
        } // namespace detail

        detail::ComponentsLoader& componentsLoader();
    } // namespace ecs
} // namespace stay


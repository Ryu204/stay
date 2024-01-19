#pragma once

#include <unordered_map>

#include <entt/meta/resolve.hpp>

#include "ecs/manager.hpp"

namespace stay
{
    class IComponentSerializer 
    {
        public:
            virtual ~IComponentSerializer() = default;
            // @brief load a component of `val` into `entity` of `manager`
            // @return `true` if the load is successful, `false` otherwise
            virtual bool deserializeInto(ecs::Manager* /*manager*/, ecs::Entity /*entity*/, const Serializable::Data& /*val*/) const = 0;
            // @brief create a json object from `entity`'s corresponding component
            virtual Serializable::Data serialize(ecs::Manager* /*manager*/, ecs::Entity /*entity*/) const = 0;
            // @brief create a dummy default component
            // @note to initialize the registry
            virtual void createDefault(ecs::Registry* /*registry*/, ecs::Entity /*entity*/) const = 0;
    };

    template <typename T, whereIs(T, ecs::Component)>
    class ComponentSerializer : public IComponentSerializer
    {
        public:
            bool deserializeInto(ecs::Manager* manager, ecs::Entity entity, const Serializable::Data& val) const override
            {
                auto& comp = manager->addComponent<T>(entity);
                return comp.fetch(val);
            }
            Serializable::Data serialize(ecs::Manager* manager, ecs::Entity entity) const override
            {
                return manager->getComponent<T>(entity).toJSONObject();
            }
            void createDefault(ecs::Registry* reg, ecs::Entity entity) const override
            {
                reg->emplace<T>(entity);
            }
    };

    class ComponentsLoader
    {
        /* 
            // Examples on a loadable object
            "componentsArray" : [
                {
                    "name" : "Render",
                    "data" : {
                        "Color" : [0.1, 0.2, 0.3, 1.0],
                        "Size" : [100.0, 200.0]
                    }
                },
                {
                    "name" : "Collider",
                    "data" : {
                        "type" : "Circle",
                        "radius" : 100.0
                    }
                }
            ]
        */
        public:
            ComponentsLoader(ecs::Manager* manager);
            ~ComponentsLoader();

            template <typename T, whereIs(T, ecs::Component)>
            void registerComponent(const std::string& name)
            {
                mLoaderList.emplace(name, std::make_unique<ComponentSerializer<T>>());
                // Trigger the creation of registry's storage
                mLoaderList.at(name)->createDefault(&mManager->getRegistryRef(), mUtilizeEntity);
                const auto& storage = mManager->getRegistryRef().storage<T>();
                mTypenameToLoader.emplace(storage.type().name(), name);
                // Clean the utilize entity
                mManager->getRegistryRef().remove<T>(mUtilizeEntity);
            }

            void loadAllComponents(ecs::Entity entity, const Serializable::Data& componentsArray);
            void loadComponent(ecs::Entity entity, const std::string& name, const Serializable::Data& componentData);
            Serializable::Data saveAllComponents(ecs::Entity entity) const;
            Serializable::Data saveComponent(ecs::Entity entity, const std::string_view& name) const;
        private:
            static const std::string& error();
            // Used to get internal name of component type in `mManager->getRegistry()`
            ecs::Entity mUtilizeEntity;
            ecs::Manager* mManager;
            std::unordered_map<std::string, Uptr<IComponentSerializer>> mLoaderList;
            std::unordered_map<std::string, std::string> mTypenameToLoader;
    };
} // namespace stay

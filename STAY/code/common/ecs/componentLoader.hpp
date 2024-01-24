#pragma once

#include "ecs/type.hpp"
#include "manager.hpp"

namespace stay
{
    class IComponentSerializer 
    {
        public:
            virtual ~IComponentSerializer() = default;
            virtual bool deserialize(ecs::Manager& /*manager*/, ecs::Entity /*entity*/, const Serializable::Data& /*val*/) const = 0;
            virtual Serializable::Data serialize(ecs::Manager& /*manager*/, ecs::Entity /*entity*/) const = 0;
            virtual bool inEntity(ecs::Manager& /* manager */, ecs::Entity /* entity */) const = 0;
    };

    template <typename T, whereIs(T, ecs::Component<T>)>
    class ComponentSerializer : public IComponentSerializer
    {
        public:
            bool deserialize(ecs::Manager& manager, ecs::Entity entity, const Serializable::Data& val) const override
            {
                auto& comp = manager.addComponent<T>(entity);
                return comp.deserialization(val);
            }
            Serializable::Data serialize(ecs::Manager& manager, ecs::Entity entity) const override
            {
                return manager.getComponent<T>(entity).serialize();
            }
            bool inEntity(ecs::Manager& manager, ecs::Entity entity) const override
            {
                return manager.hasComponent<T>(entity);
            }
    };

    // This class throws like a maniac, use with appropriate `try/catch` when loading components
    class ComponentsLoader
    {
        public:
            template <typename T, whereIs(T, ecs::Component<T>)>
            void registerComponent(const std::string& name)
            {
                assert(mLoaderList.find(name) == mLoaderList.end() && "component name registered twice");
                mLoaderList.emplace(name, std::make_unique<ComponentSerializer<T>>());                
            }

            void loadAllComponents(ecs::Manager& manager, ecs::Entity entity, const Serializable::Data& componentsArray);
            void loadComponent(ecs::Manager& manager, ecs::Entity entity, const std::string& name, const Serializable::Data& componentData);
            Serializable::Data saveAllComponents(ecs::Manager& manager, ecs::Entity entity) const;
            Serializable::Data saveComponent(ecs::Manager& manager, ecs::Entity entity, const std::string& name) const;
        private:
            std::unordered_map<std::string, Uptr<IComponentSerializer>> mLoaderList;
    };

    inline ComponentsLoader& componentsLoader() {
        static ComponentsLoader res;
        return res;
    }
} // namespace stay

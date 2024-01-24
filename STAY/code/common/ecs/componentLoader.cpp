#include "componentLoader.hpp"
#include "componentLoader.impl.hpp" // IWYU pragma: keep
#include "utility/error.hpp"

namespace stay
{
    namespace ecs
    {
        namespace detail
        {
            void ComponentsLoader::loadAllComponents(ecs::Manager& manager, ecs::Entity entity, const Serializable::Data& componentsArray)
            {
                const bool isArray = componentsArray.type() == nlohmann::json::value_t::array;
                utils::throwIfFalse(isArray, "components list is not an array");
                for (const auto& obj : componentsArray)
                {
                    const std::string name = obj["name"].get<std::string>();
                    const bool nameFound = mLoaderList.find(name) != mLoaderList.end();
                    utils::throwIfFalse(nameFound, "name \"" + name + "\" not recognized");
                    loadComponent(manager, entity, name, obj["data"]);
                }
            }

            void ComponentsLoader::loadComponent(ecs::Manager& manager, ecs::Entity entity, const std::string& name, const Serializable::Data& componentData)
            {
                const auto& loader = *mLoaderList.at(name);
                auto loadSucceeded = loader.deserialize(manager, entity, componentData);
                utils::throwIfFalse(loadSucceeded, "cannot load data of \"" + name + "\"");
            }

            Serializable::Data ComponentsLoader::saveAllComponents(ecs::Manager& manager, ecs::Entity entity) const
            {
                Serializable::Data arr(nlohmann::json::value_t::array);
                for (const auto& [name, loader] : mLoaderList)
                {
                    const bool hasComponent = loader->inEntity(manager, entity);
                    if (!hasComponent)
                        continue;
                    arr.emplace_back(saveComponent(manager, entity, name));   
                }
                return arr;
            }

            Serializable::Data ComponentsLoader::saveComponent(ecs::Manager& manager, ecs::Entity entity, const std::string& name) const
            {
                Serializable::Data res;
                res["name"] = name;
                const auto& loader = *mLoaderList.at(name);
                res["data"] = loader.serialize(manager, entity);
                return res;
            }
        } // namespace detail
    } // namespace ecs
} // namespace stay

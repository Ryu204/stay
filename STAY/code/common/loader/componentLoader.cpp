#include "componentLoader.hpp"
#include "../utility/error.hpp"

namespace stay
{
    ComponentsLoader::ComponentsLoader(ecs::Manager* manager)
        : mUtilizeEntity(manager->getRegistryRef().create())
        , mManager(manager)
    {}

    ComponentsLoader::~ComponentsLoader()
    {
        mManager->getRegistryRef().destroy(mUtilizeEntity);
    }

    void ComponentsLoader::loadAllComponents(ecs::Entity entity, const Serializable::Data& componentsArray)
    {
        const bool isArray = componentsArray.type() == nlohmann::json::value_t::array;
        utils::throwIfFalse(isArray, "not an array");
        for (const auto& obj : componentsArray)
        {
            const std::string name = obj["name"].get<std::string>();
            const bool nameFound = mLoaderList.find(name) != mLoaderList.end();
            utils::throwIfFalse(nameFound, "name \"" + name + "\" not recognized");
            loadComponent(entity, name, obj["data"]);
        }
    }

    void ComponentsLoader::loadComponent(ecs::Entity entity, const std::string& name, const Serializable::Data& componentData)
    {
        const auto& loader = *mLoaderList.at(name);
        auto loadSucceeded = loader.deserializeInto(mManager, entity, componentData);
        utils::throwIfFalse(loadSucceeded, "cannot load data of \"" + name + "\"");
    }

    Serializable::Data ComponentsLoader::saveAllComponents(ecs::Entity entity) const
    {
        Serializable::Data arr(nlohmann::json::value_t::array);
        const auto& reg = mManager->getRegistryRef();
        for (const auto [id, storage] : reg.storage())
        {
            auto type = storage.type();
            const bool hasComponent = storage.contains(entity);
            if (!hasComponent)
                continue;
            arr.emplace_back(saveComponent(entity, type.name()));   
        }
        return arr;
    }

    Serializable::Data ComponentsLoader::saveComponent(ecs::Entity entity, const std::string_view& name) const
    {
        Serializable::Data res;
        const std::string nameStr(name);
        res["name"] = mTypenameToLoader.at(nameStr);
        const auto& loader = *mLoaderList.at(mTypenameToLoader.at(std::string(name)));
        res["data"] = loader.serialize(mManager, entity);
        return res;
    }
} // namespace stay

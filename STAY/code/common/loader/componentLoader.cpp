#include "componentLoader.hpp"
#include "../utility/error.hpp"

namespace stay
{
    const std::string& ComponentsLoader::error()
    {
        static std::string res("Data file is broken");
        return res;
    }

    ComponentsLoader::ComponentsLoader(ecs::Manager* manager)
        : mUtilizeEntity(manager->getRegistryRef().create())
        , mManager(manager)
    {}

    ComponentsLoader::~ComponentsLoader()
    {
        mManager->getRegistryRef().destroy(mUtilizeEntity);
    }

    void ComponentsLoader::loadAllComponents(ecs::Entity entity, const Json::Value& componentsArray)
    {
        const bool isArray = componentsArray.type() == Json::ValueType::arrayValue;
        utils::throwIfFalse(isArray, error());
        for (Json::ArrayIndex i = 0; i < componentsArray.size(); ++i) // NOLINT
        {
            const auto& obj = componentsArray[i];
            const std::string name = obj["name"].asString();
            bool nameFound = mLoaderList.find(name) != mLoaderList.end();
            utils::throwIfFalse(nameFound, error());
            loadComponent(entity, name, obj["data"]);
        }
    }

    void ComponentsLoader::loadComponent(ecs::Entity entity, const std::string& name, const Json::Value& componentData)
    {
        const auto& loader = *mLoaderList.at(name);
        auto loadSucceeded = loader.deserializeInto(mManager, entity, componentData);
        utils::throwIfFalse(loadSucceeded, error());
    }

    Json::Value ComponentsLoader::saveAllComponents(ecs::Entity entity) const
    {
        Json::Value arr(Json::ValueType::arrayValue);
        const auto& reg = mManager->getRegistryRef();
        for (const auto [id, storage] : reg.storage())
        {
            auto type = storage.type();
            bool hasComponent = storage.contains(entity);
            if (!hasComponent)
                continue;
            arr.append(saveComponent(entity, type.name()));   
        }
        return arr;
    }

    Json::Value ComponentsLoader::saveComponent(ecs::Entity entity, const std::string_view& name) const
    {
        Json::Value res;
        std::string nameStr(name);
        res["name"] = mTypenameToLoader.at(nameStr);
        const auto& loader = *mLoaderList.at(mTypenameToLoader.at(std::string(name)));
        res["data"] = loader.serialize(mManager, entity);
        return res;
    }
} // namespace stay

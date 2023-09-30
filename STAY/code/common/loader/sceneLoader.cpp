#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "sceneLoader.hpp"

namespace stay
{
   
    SceneLoader::SceneLoader(ecs::Manager* manager, Path&& filepath)
        : mFile(filepath)
        , mLoader(manager)
    {}

    // @return Root node
    Uptr<Node> SceneLoader::load()
    {
        Json::Value all;
        std::ifstream(mFile) >> all;
        if (!all["root"].isInt())
            throw std::runtime_error(error() + " (no \"root\" id)");
        auto res = std::make_unique<Node>(static_cast<ecs::Entity>(all["root"].asInt()));
        all = all["entities"];
        static std::unordered_map<ecs::Entity, ecs::Entity> parents;
        parents.clear();
        for (const auto& entity : all)
        {
            if (!(entity["id"].isInt() && entity["parent"].isInt()))
                throw std::runtime_error(error() + " (no entity id)");
            auto id = static_cast<ecs::Entity>(entity["id"].asInt());
            res->createChild(id);
            parents.emplace(id, static_cast<ecs::Entity>(entity["parent"].asInt()));
            mLoader.loadAllComponents(id, entity["components"]);
        }
        for (const auto [child, parent] : parents)
        {
            Node::getNode(child)->setParent(parent);
        }
        return std::move(res);
    }

    std::string& SceneLoader::error()
    {
        static std::string res("Entity ID in data file is faulty");
        return res;
    }
} // namespace stay

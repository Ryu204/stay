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
        auto file = mFile/"in";
        Json::Value data;
        {
            std::ifstream reader(file);
            if (!reader.good())
                throw std::runtime_error("Cannot open data file");
            reader >> data;
        }
        if (!data["root"].isInt())
            throw std::runtime_error(error() + " (no \"root\" id)");
        auto root = std::make_unique<Node>(static_cast<ecs::Entity>(data["root"].asInt()));
        data = data["entities"];

        mParentOf.clear();

        for (const auto& entity : data)
        {
            bool hasValidIDs = entity["id"].isInt() && entity["parent"].isInt();
            if (!hasValidIDs)
                throw std::runtime_error(error() + " (entity or parent id not found)");
            auto id = static_cast<ecs::Entity>(entity["id"].asInt());
            auto* created = root->createChild(id);
            bool hasTransform = created->localTransform().fetch(entity["transform"]);
            if (!hasTransform)
                throw std::runtime_error(error() + " (transform data not found)");
            mParentOf[id] = static_cast<ecs::Entity>(entity["parent"].asInt());
            mLoader.loadAllComponents(id, entity["components"]);
        }
        for (const auto [child, parent] : mParentOf)
        {
            Node::getNode(child)->setParent(parent);
        }
        mParentOf.clear();
        return std::move(root);
    }

    std::string& SceneLoader::error()
    {
        static std::string res("Entity data in file is faulty");
        return res;
    }

    void SceneLoader::save(Node* root) const
    {
        auto output = mFile/"out";
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";
        builder["commentStyle"] = "None";
        std::ofstream(output) << Json::writeString(builder, createSaveObject(root)) << std::endl;
    }

    Json::Value SceneLoader::createSaveObject(Node* root) const
    {
        Json::Value res;
        res["root"] = static_cast<int>(root->entity());
        res["entities"] = Json::Value(Json::arrayValue);

        const auto saveToEntities = [root, this, &res](Node* node) -> void {
            if (node == root)
                return;
            Json::Value entity;
            entity["id"] = static_cast<int>(node->entity());
            entity["parent"] = static_cast<int>(node->parent()->entity());
            entity["transform"] = node->localTransform().toJSONObject();
            entity["components"] = mLoader.saveAllComponents(node->entity());
            res["entities"].append(entity);
        };

        root->visit(saveToEntities);

        return res;
    }
} // namespace stay

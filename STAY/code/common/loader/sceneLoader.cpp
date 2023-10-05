#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>

#include "sceneLoader.hpp"
#include "LDtk/rawSceneLoader.hpp"

namespace stay
{
   
    SceneLoader::SceneLoader(ecs::Manager* manager, Path&& filepath)
        : mFile(filepath)
        , mLoader(manager)
    {}

    // @return Root node
    Uptr<Node> SceneLoader::load()
    {
        try 
        {
            std::string log;
            auto* maybeRes = tryLoad(log);
            if (maybeRes != nullptr)
                return Uptr<Node>(maybeRes);
            else
                throw std::runtime_error(log);
        }
        catch (std::exception& e)
        {
            RawSceneLoader altLoader;
            auto res = altLoader.load(mFile/"in.ldtk", e.what());
            save(res.get(), false);
            return std::move(res);
        }
    }

    Node* SceneLoader::tryLoad(std::string& log)
    {
        log = "";
        auto data = openFile();
        if (!data["root"].isInt())
            throw std::runtime_error("no \"root\" id");
        if (!data["maxEntity"].isInt())
            throw std::runtime_error("no \"maxEntity\"");
        auto rootID = static_cast<ecs::Entity>(data["root"].asInt());
        auto sceneMaxEntity = data["maxEntity"].asInt();
        {
            bool exceededThreshold = sceneMaxEntity > (1 << 20);
            if (exceededThreshold)
                throw std::runtime_error("internal error");
        }
        Uptr<Node> reserver = std::make_unique<Node>(rootID);
        Node fakeRoot(static_cast<ecs::Entity>(sceneMaxEntity + 1));
        Node* underRoot = fakeRoot.createChild(static_cast<ecs::Entity>(sceneMaxEntity + 2));
        data = data["entities"];
        mParentOf.clear();
        for (const auto& entity : data)
            loadEntity(underRoot, entity);
        
        reserver.reset();
        Node* rootPtr = new Node(rootID);
        underRoot->setParent(rootPtr);
        mParentOf.clear();
        for (const auto [child, parent] : mParentOf)
            Node::getNode(child)->setParent(parent);
        return rootPtr;
    }

    Json::Value SceneLoader::openFile()
    {
        auto file = mFile/"in";
        try
        {
            std::ifstream reader(file);
            Json::Value res;
            reader >> res;
            return res;
        }
        catch(std::exception& e)
        {
            throw std::runtime_error("Cannot open file " + file.string() + ":\n" + e.what());
        }
    }

    void SceneLoader::loadEntity(Node* currentRoot, const Json::Value& data)
    {
        bool hasValidIDs = data["id"].isInt() && data["parent"].isInt();
        if (!hasValidIDs)
            throw std::runtime_error("entity or parent id not found");
        auto id = static_cast<ecs::Entity>(data["id"].asInt());
        auto* created = currentRoot->createChild(id);
        bool hasTransform = created->localTransform().fetch(data["transform"]);
        if (!hasTransform)
            throw std::runtime_error("transform data not found");
        mParentOf[id] = static_cast<ecs::Entity>(data["parent"].asInt());
        try
        {
            mLoader.loadAllComponents(id, data["components"]);
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error(std::string("components data is broken: ") + e.what());
        }
    }

    void SceneLoader::save(Node* root, bool overrideIn) const
    {
        auto output = overrideIn ? mFile/"in" : mFile/"out";
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
        int maxEntity = res["root"].asInt();

        const auto saveToEntities = [&](Node* node) -> void {
            if (node == root)
                return;
            Json::Value entity;
            entity["id"] = static_cast<int>(node->entity());
            entity["parent"] = static_cast<int>(node->parent()->entity());
            entity["transform"] = node->localTransform().toJSONObject();
            entity["components"] = mLoader.saveAllComponents(node->entity());
            res["entities"].append(entity);
            maxEntity = std::max(maxEntity, static_cast<int>(node->entity()));
        };

        root->visit(saveToEntities);
        res["maxEntity"] = maxEntity;
        return res;
    }
} // namespace stay

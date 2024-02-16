#include "sceneLoader.hpp"
#include "iLoader.hpp"
#include "ecs/component.hpp"
#include "utility/error.hpp"
#include "world/node.hpp"

#include <fstream>

namespace stay
{
    SceneLoader::SceneLoader(ecs::Manager& manager, Path&& filepath)
        : mFile(filepath)
        , mManager(manager)
    {}

    // @return Root node
    Uptr<Node> SceneLoader::load()
    {
        try 
        {
            return tryLoad();
        }
        catch (std::exception& e)
        {
            auto& altLoader = detail::loader().getLoader();
            auto res = std::make_unique<Node>(ecs::Entity{1});
            assert(static_cast<int>(res->entity()) == 1 && "entity 1 is used");
            try
            {
                altLoader.load(mFile/"in.ldtk", res.get());
                return std::move(res);
            }
            catch (std::exception& e2)
            {
                throw std::runtime_error(
                    std::string("Load failed: ") + e.what() 
                    +  "\nOpened alternate file but failed: " + e2.what()
                );
            }
        }
    }

    Uptr<Node> SceneLoader::tryLoad()
    {
        auto data = openFile();
        if (!data["topId"].is_number_integer())
            throw std::runtime_error("no top id");
        const auto topId = static_cast<ecs::Entity>(data["topId"].get<int>());
        auto topNode = std::make_unique<Node>(topId);
        if (!topNode->localTransform().deserialization(data["topNode"]["transform"]))
            throw std::runtime_error("no top node transform");
        try 
        {
            ecs::componentsLoader().loadAllComponents(mManager, topId, data["topNode"]["components"]);
        }
        catch (std::exception& e)
        {
            throw std::runtime_error(std::string("top node components list is broken: ") + e.what());
        }
        mParentOf.clear();
        utils::throwIfFalse(data["entities"].is_array(), "entities object must be an array");
        for (const auto& entity : data["entities"])
            loadEntity(*topNode, entity);
        for (const auto [child, parent] : mParentOf)
            Node::getNode(child)->setParent(parent);
        return std::move(topNode);
    }

    Serializable::Data SceneLoader::openFile()
    {
        auto file = mFile/"in";
        try
        {
            std::ifstream reader(file);
            Serializable::Data res;
            reader >> res;
            return res;
        }
        catch(std::exception& e)
        {
            throw std::runtime_error("Cannot open file " + file.string() + ":\n" + e.what());
        }
    }

    void SceneLoader::loadEntity(Node& parent, const Serializable::Data& data)
    {
        const bool hasValidIDs = data["id"].is_number_integer() && data["parent"].is_number_integer();
        if (!hasValidIDs)
            throw std::runtime_error("entity or parent id not found");
        const auto id = static_cast<ecs::Entity>(data["id"].get<int>());
        auto* created = parent.createChild(id);
        const bool hasTransform = created->localTransform().deserialization(data["transform"]);
        if (!hasTransform)
            throw std::runtime_error("transform data not found");
        mParentOf[id] = static_cast<ecs::Entity>(data["parent"].get<int>());
        try
        {
            ecs::componentsLoader().loadAllComponents(mManager, id, data["components"]);
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error(std::string("components data is broken: ") + e.what());
        }
    }

    void SceneLoader::save(Node* root, bool overrideIn) const
    {
        auto output = overrideIn ? mFile/"in" : mFile/"out";
        std::ofstream(output) << createSaveObject(root).dump(1, '\t', true) << '\n';
    }

    Serializable::Data SceneLoader::createSaveObject(Node* topNode) const
    {
        auto& loader = ecs::componentsLoader();
        Serializable::Data res;
        res["topId"] = static_cast<int>(topNode->entity());
        res["topNode"] = {
            {"transform", topNode->localTransform().serialize()},
            {"components", loader.saveAllComponents(mManager, topNode->entity())}
        };
        res["entities"] = Serializable::Data(nlohmann::json::value_t::array);
        const auto saveToEntities = [this, topNode, &res, &loader](Node* node) -> void {
            if (node == topNode) 
                return;
            res["entities"].emplace_back(Serializable::Data{
                {"id", static_cast<int>(node->entity())},
                {"parent", static_cast<int>(node->parent()->entity())},
                {"transform", node->localTransform().serialize()},
                {"components", loader.saveAllComponents(mManager, node->entity())}
            });
        };
        topNode->visit(saveToEntities);
        return res;
    }
} // namespace stay

#include "rawSceneLoader.hpp"
#include "../../../game/component/list.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace stay
{
    std::string &RawSceneLoader::error()
    {
        static std::string res("Attempted to open alt file but failed");
        return res;
    }

    void RawSceneLoader::init(const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        mPxPerMeter = level.getFieldInstances().at(0).getValue().get<float>();
        mLayerOffset = Vector2(layer.getPxTotalOffsetX() + level.getWorldX(), layer.getPxTotalOffsetY() + level.getWorldY());
        mTileSize = layer.getGridSize();
        mLayerSize = Vector2(layer.getCWid(), layer.getCHei()) * mTileSize;
    }

    Vector2 RawSceneLoader::fileToWorld(const Vector2& pos) const
    {
        return Vector2(pos.x, -pos.y) / mPxPerMeter;
    }

    void RawSceneLoader::loadTiles(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        for (const auto& tile : layer.getAutoLayerTiles())
        {
            auto* entity = currentRoot->createChild();
            const auto& tilePos = Vector2(tile.getPx()[0], tile.getPx()[1]) + Vector2(0.5F, 0.5F) * mPxPerMeter + mLayerOffset;
            entity->localTransform().setPosition(fileToWorld(tilePos));
            entity->addComponent<comp::Render>(Color(0x00FF00FF), Vector2(mTileSize, mTileSize) / mPxPerMeter);
        }
    }
// NOLINTBEGIN
    void RawSceneLoader::loadColliders(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        const auto& entity = layer.getEntityInstances().at(0);
        const auto& list = entity.getFieldInstances().at(0).getValue();
        std::vector<Vector2> chainShape;
        for (const auto& i : list)
        {
            auto point = i.get<ldtk::GridPoint>();
            Vector2 filePos = Vector2(point.getCx(), point.getCy()) * mTileSize + mLayerOffset;
            chainShape.emplace_back(fileToWorld(filePos));
        }
        auto* node = currentRoot->createChild();
        node->addComponent<phys::RigidBody>(/*position = */Vector2());
        node->addComponent<phys::Collider>(phys::Chain(chainShape));
    }
// NOLINTEND

    Uptr<Node> RawSceneLoader::load(Path &&filename, const std::string& switchReason)
    {
        try
        {
            std::ifstream reader;
            reader.open(filename);
            std::stringstream buffer;
            buffer << reader.rdbuf();
            ldtk::Coordinate coord = nlohmann::json::parse(buffer.str());
            reader.close();

            Uptr<Node> root = std::make_unique<Node>();
            const auto& level = coord.getLevels()[0];
            const auto layers = level.getLayerInstances();
            const auto& platform = layers->at(1);
            const auto& collider = layers->at(0);

            loadTiles(root.get(), level, platform);
            loadColliders(root.get(), level, collider);

            return std::move(root);
        }
        catch (std::exception& e)
        {
            throw std::runtime_error(switchReason + "\n" + error() + ":\n" + e.what());
        }
    }
} // namespace stay

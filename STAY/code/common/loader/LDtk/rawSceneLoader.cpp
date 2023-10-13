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

    void RawSceneLoader::loadColliders(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        const auto& entity = layer.getEntityInstances().at(0);
        const auto& list = entity.getFieldInstances().at(0).getValue();
        std::vector<Vector2> chainShape;
        for (const auto& i : list)
        {
            auto point = i.get<ldtk::GridPoint>();
            const Vector2 filePos = Vector2(point.getCx(), point.getCy()) * mTileSize + mLayerOffset;
            chainShape.emplace_back(fileToWorld(filePos));
        }
        auto* node = currentRoot->createChild();
        node->addComponent<phys::RigidBody>(/*position = */Vector2());
        phys::Material mat(1.F, 1.F, 0.F);
        node->addComponent<phys::Collider>(phys::Chain(chainShape), mat);
    }

    void RawSceneLoader::loadPlayer(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        // node
        const auto& player = layer.getEntityInstances().at(0);
        auto* node = currentRoot->createChild();
        // RigidBody
        Vector2 pos = Vector2(player.getPx()[0], player.getPx()[1]) + mLayerOffset;
        pos = fileToWorld(pos);
        auto& rgbody = node->addComponent<phys::RigidBody>(pos, 0.F, phys::BodyType::DYNAMIC);
        rgbody.setGravityScale(player.getFieldInstances().at(2).getValue().get<float>());
        rgbody.setLinearDamping(player.getFieldInstances().at(5).getValue().get<float>());
        // Collider
        float friction = player.getFieldInstances().at(6).getValue().get<float>();
        phys::Material mat(1.0F, friction, 0.0F);
        auto& col = node->addComponent<phys::Collider>(phys::Circle(Vector2(), player.getWidth() / 2.F / mPxPerMeter), mat);
        col.setLayer("Player");
        // Player
        auto& cmp = node->addComponent<Player>();
        cmp.moveStrength = player.getFieldInstances().at(0).getValue().get<float>();
        cmp.jumpHeight = player.getFieldInstances().at(1).getValue().get<float>();
        cmp.oppositeScale = player.getFieldInstances().at(7).getValue().get<float>();
        // Hook
        auto& hk = node->addComponent<Hook>();
        hk.speed = player.getFieldInstances().at(3).getValue().get<float>();
        hk.cooldown = player.getFieldInstances().at(4).getValue().get<float>();
    }

    Uptr<Node> RawSceneLoader::load(Path &&filename, const std::string& switchReason)
    {
        try
        {
            std::ifstream reader;
            reader.open(filename);
            std::stringstream buffer;
            buffer << reader.rdbuf();
            const ldtk::Coordinate coord = nlohmann::json::parse(buffer.str());
            reader.close();

            Uptr<Node> root = std::make_unique<Node>();
            const auto& level = coord.getLevels()[0];
            const auto layers = level.getLayerInstances();
            const auto& platform = layers->at(1);
            const auto& collider = layers->at(0);
            const auto& player = layers->at(2);

            loadTiles(root.get(), level, platform);
            loadColliders(root.get(), level, collider);
            loadPlayer(root.get(), level, player);

            return std::move(root);
        }
        catch (std::exception& e)
        {
            throw std::runtime_error(switchReason + "\n" + error() + ":\n" + e.what());
        }
    }
} // namespace stay

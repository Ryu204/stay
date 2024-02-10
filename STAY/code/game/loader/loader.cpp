#include "loader.hpp"
#include "../component/list.hpp"
#include "quicktype.hpp"
#include "world/node.hpp"

#include <fstream>
#include <sstream>

namespace stay
{
    void Loader::init(const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        mPxPerMeter = level.getFieldInstances().at(0).getValue().get<float>();
        mLayerOffset = Vector2(layer.getPxTotalOffsetX() + level.getWorldX(), layer.getPxTotalOffsetY() + level.getWorldY());
        mTileSize = layer.getGridSize();
        mLayerSize = Vector2(layer.getCWid(), layer.getCHei()) * mTileSize;
    }

    Vector2 Loader::fileToWorld(const Vector2& pos) const
    {
        return Vector2(pos.x, -pos.y) / mPxPerMeter;
    }

    void Loader::loadTiles(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        for (const auto& tile : layer.getAutoLayerTiles())
        {
            auto* entity = currentRoot->createChild();
            const auto& tilePos = Vector2(tile.getPx()[0], tile.getPx()[1]) + Vector2(0.5F, 0.5F) * mPxPerMeter + mLayerOffset;
            entity->localTransform().setPosition(fileToWorld(tilePos));
            entity->addComponent<Render>(Color(0x00FF00FF), Vector2(mTileSize, mTileSize) / mPxPerMeter);
        }
    }

    void Loader::loadColliders(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        for (const auto& entity : layer.getEntityInstances())
        {
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
    }

    void Loader::loadPlayer(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer)
    {
        init(level, layer);
        // node
        const auto& player = layer.getEntityInstances().at(0);
        auto* node = currentRoot->createChild();
        auto* skin = node->createChild();
        // RigidBody
        Vector2 pos = Vector2(player.getPx()[0], player.getPx()[1]) + mLayerOffset;
        pos = fileToWorld(pos);
        auto& hookBody = node->addComponent<phys::RigidBody>(pos, 0.F, phys::BodyType::DYNAMIC);
        auto& skinBody = skin->addComponent<phys::RigidBody>(pos, 0.F, phys::BodyType::DYNAMIC);
        skinBody.setGravityScale(player.getFieldInstances().at(2).getValue().get<float>());
        skinBody.setHorizontalDamping(player.getFieldInstances().at(5).getValue().get<float>());
        skinBody.setLinearDamping(player.getFieldInstances().at(8).getValue().get<float>());
        skinBody.setFixedRotation(true);
        // Skin
        const auto friction = player.getFieldInstances().at(6).getValue().get<float>();
        const auto radius = player.getWidth() / 2.F / mPxPerMeter;
        phys::Box skinShape{Vector2{}, Vector2{1.15F * 2.F * radius, 1.35F * 2.F * radius}, 0.F};
        phys::Material playerMat{1.F, friction, 0.F};
        auto& skinCollider = skin->addComponent<phys::Collider>(skinShape, playerMat);
        skinCollider.setLayer("Player");
        skin->addComponent<phys::Joint>().start(phys::JointInfo{node->entity(), false, phys::Revolute{pos}});
        // Hook collider
        phys::Material light{0.1F};
        auto& hookCollider = node->addComponent<phys::Collider>(phys::Circle{Vector2{}, radius}, light);
        hookCollider.setLayer("Player");
        // Player
        auto& cmp = node->addComponent<Player>();
        cmp.moveStrength = player.getFieldInstances().at(0).getValue().get<float>();
        cmp.jumpHeight = player.getFieldInstances().at(1).getValue().get<float>();
        cmp.oppositeScale = player.getFieldInstances().at(7).getValue().get<float>();
        cmp.airScale = player.getFieldInstances().at(9).getValue().get<float>();
        // Hook
        auto& hk = node->addComponent<Hook>();
        hk.props.speed = player.getFieldInstances().at(3).getValue().get<float>();
        hk.props.cooldown = player.getFieldInstances().at(4).getValue().get<float>();
        hk.props.ropeLength = player.getFieldInstances().at(10).getValue().get<float>();
        hk.props.pullSpeed = player.getFieldInstances().at(11).getValue().get<float>();
        // Debug
        node->addComponent<PlayerDebug>();
        // Dash
        auto& dash = node->addComponent<Dash>();
        dash.velocity = player.getFieldInstances().at(12).getValue().get<float>();
        dash.length = player.getFieldInstances().at(13).getValue().get<float>();
        dash.cooldown = player.getFieldInstances().at(14).getValue().get<float>();
        dash.postBrake = player.getFieldInstances().at(15).getValue().get<float>();
    }

    void Loader::load(const std::filesystem::path& filename, Node* node)
    {
        std::ifstream reader;
        reader.open(filename);
        std::stringstream buffer;
        buffer << reader.rdbuf();
        const ldtk::Coordinate coord = nlohmann::json::parse(buffer.str());
        reader.close();

        const auto& level = coord.getLevels()[0];
        const auto layers = level.getLayerInstances();
        const auto& platform = layers->at(1);
        const auto& collider = layers->at(0);
        const auto& player = layers->at(2);

        loadTiles(node, level, platform);
        loadColliders(node, level, collider);
        loadPlayer(node, level, player);
    }
} // namespace stay

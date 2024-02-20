#include "loader.hpp"

#include "stay/ecs/component.hpp"
#include "stay/components.hpp"
#include "../component/player.hpp"
#include "../component/dash.hpp"
#include "../component/playerDebug.hpp"
#include "../component/hook.hpp"

namespace stay
{
    void Loader::load(std::filesystem::path&& filename, Node* root)
    {
        ldtk::Project project;
        project.loadFromFile(filename.string());
        const auto& world = project.getWorld();
        const auto& level = world.getLevel("main");

        mDetail.pixelsPerMeter = level.getField<float>("pixelsPerMeter").value();
        mDetail.worldOffset = -0.5F * Vector2::from(level.size);

        const auto& bgr = level.getLayer("background");
        loadTileset(root, bgr);

        const auto& colliders = level.getLayer("colliders");
        loadColliders(root, colliders);

        const auto& objects = level.getLayer("entities");
        loadPlayer(root, objects);
    }

    void Loader::loadTileset(Node* parent, const ldtk::Layer& layer) const
    {
        assert(layer.hasTileset() && "not tiled layer");
        const auto tileSize = layer.getTileset().tile_size / mDetail.pixelsPerMeter;
        for (const auto& tile : layer.allTiles())
        {
            auto* entity = parent->createChild();
            Transform tf{toWorldPosition(Vector2::from(tile.getPosition()))};
            entity->setGlobalTransform(tf);
            const auto tRect = tile.getTextureRect();
            const Rect textureRect{Vector2{tRect.x, tRect.y}, Vector2{tRect.x + tRect.width, tRect.y + tRect.height}};
            const Vector2 pivot{0, 0};
            TextureInfo info{ "mossy", textureRect, pivot };
            entity->addComponent<Render>(Color{0xFFFFFFFF}, Vector2{tileSize, tileSize}, 0, info);
        }
    }

    void Loader::loadColliders(Node* parent, const ldtk::Layer& layer) const 
    {
        const float tileSize = layer.getCellSize();
        for (const auto& collider : layer.allEntities())
        {
            assert(collider.getName() == "collider" && "not a collider entity");
            std::vector<Vector2> chainShape;
            const auto position = toWorldPosition(Vector2::from(collider.getPosition()));
            for (const auto& point : collider.getArrayField<ldtk::IntPoint>("chain"))
            {
                const Vector2 gridPosition = Vector2::from(point.value()) + Vector2{0.5F, 0.5F};
                const auto pointPosition = toWorldPosition(tileSize * gridPosition);
                chainShape.emplace_back(pointPosition - position);
            }
            auto* entity = parent->createChild();
            phys::Material mat{1.F, 1.F, 0.F};
            entity->addComponent<phys::RigidBody>(position);
            entity->addComponent<phys::Collider>(phys::Chain{std::move(chainShape)}, mat);
        }
    }

    void Loader::loadPlayer(Node* parent, const ldtk::Layer& layer) const 
    {
        const auto& playerList = layer.getEntitiesByName("player");
        assert(playerList.size() == 1 && "exactly 1 player must present");
        const auto& player = playerList.front().get();
        const auto position = toWorldPosition(Vector2::from(player.getPosition()));

        auto* playerNode = parent->createChild();
        
        playerNode->addComponent<phys::RigidBody>(position, 0.F, phys::BodyType::DYNAMIC);
        phys::Material light{0.1F};
        const auto size = Vector2{player.getSize().x / 1.15F, player.getSize().y} / mDetail.pixelsPerMeter;
        auto& playerBody = playerNode->addComponent<phys::Collider>(phys::Circle{Vector2{}, size.x / 2.F / 1.2F}, light);
        playerBody.setLayer("Player");

        auto& stats = playerNode->addComponent<Player>();
        stats.moveStrength = player.getField<float>("moveStrength").value();
        stats.jumpHeight = player.getField<float>("jumpHeight").value();
        stats.oppositeScale = player.getField<float>("oppositeScale").value();
        stats.airScale = player.getField<float>("airScale").value();

        auto& hook = playerNode->addComponent<Hook>();
        hook.props.speed = player.getField<float>("bulletSpeed").value();
        hook.props.cooldown = player.getField<float>("bulletCooldown").value();
        hook.props.ropeLength = player.getField<float>("ropeLength").value();
        hook.props.pullSpeed = player.getField<float>("pullSpeed").value();

        playerNode->addComponent<PlayerDebug>();

        auto& dash = playerNode->addComponent<Dash>();
        dash.velocity = player.getField<float>("dashSpeed").value();
        dash.length = player.getField<float>("dashLength").value();
        dash.cooldown = player.getField<float>("dashCooldown").value();
        dash.postBrake = player.getField<float>("postBrake").value();

        auto* skin = playerNode->createChild();
        auto& skinBody = skin->addComponent<phys::RigidBody>(position, 0.F, phys::BodyType::DYNAMIC);
        skinBody.setGravityScale(player.getField<float>("gravityScale").value());
        skinBody.setHorizontalDamping(player.getField<float>("HDamping").value());
        skinBody.setLinearDamping(player.getField<float>("damping").value());
        skinBody.setFixedRotation(true);

        const phys::Material playerMaterial{1.F, player.getField<float>("friction").value(), 0.F};
        const phys::Box skinShape{Vector2{}, size};
        auto& skinCollider = skin->addComponent<phys::Collider>(skinShape, playerMaterial);
        skinCollider.setLayer("Player");
        skin->addComponent<phys::Joint>().start(phys::JointInfo{playerNode->entity(), false, phys::Revolute{position}});

        const auto playerRect = [&player](){
            const auto& tmp = player.getTextureRect();
            return Rect{Vector2{tmp.x, tmp.y}, Vector2{tmp.x + tmp.width, tmp.y + tmp.height}};
        }();
        TextureInfo skinTexture{
            "player", playerRect, Vector2{0.5F, 0.5F}
        };
        skin->addComponent<Render>(Color{0xFFFFFFFF}, size, -2, skinTexture);
    }

    Vector2 Loader::toWorldPosition(const Vector2& filePosition) const
    {
        const auto screenSpace = (filePosition + mDetail.worldOffset) / mDetail.pixelsPerMeter;
        auto worldSpace = Vector2{screenSpace.x, -screenSpace.y};
        return std::move(worldSpace);
    }
} // namespace stay

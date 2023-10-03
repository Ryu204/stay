#include "rawSceneLoader.hpp"
#include "../../../game/component/list.hpp"
#include "quicktype.hpp"

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
            const auto& plRender = layers->at(1);
            const float ppm = level.getFieldInstances()[0].getValue().get<float>();
            const auto size = plRender.getGridSize() / ppm;
            const Vector2 offset(plRender.getPxTotalOffsetX(), plRender.getPxTotalOffsetY());
            const Vector2 levelSize(plRender.getCWid() * size, plRender.getCHei() * size);
            for (const auto& tile : plRender.getAutoLayerTiles())
            {
                auto* entity = root->createChild();
                Vector2 pos = (Vector2(tile.getPx()[0], tile.getPx()[1]) + offset) / ppm - levelSize / 2.F;
                pos.y = -pos.y;
                entity->localTransform().setPosition(pos);
                entity->addComponent<comp::Render>(Color(0x00FF00FF), Vector2(size, size));
            }
            return std::move(root);
        }
        catch (std::exception& e)
        {
            throw std::runtime_error(switchReason + "\n" + error() + ":\n" + e.what());
        }
    }
} // namespace stay

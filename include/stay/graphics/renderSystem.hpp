#pragma once

#include <SFML/Graphics.hpp>

#include "stay/ecs/systemTypes.hpp"
#include "stay/ecs/system.hpp"
#include "zOrdering.hpp"

/*
    Render the whole scene by traversing every node using BFS
*/

namespace stay
{
    namespace sys
    {
        struct RenderSystem 
            : public ecs::RenderSystem
            , public ecs::InitSystem
            , public ecs::ConfigurableSystem
            , public ecs::System
        {
                REGISTER_SYSTEM(RenderSystem)
                RenderSystem(ecs::Manager* manager);
                void init(ecs::SystemContext& context) override;
                void render(RTarget* target, Node* root) override;
                bool loadConfig(const Serializable::Data& data) override;
            private:
                void drawObjects(RTarget* target);
                void traverse(Node* node, RTarget* target);

                TextureManager* mTextures;
                std::vector<detail::ZOrderPack> mRenderObjects;
                sf::VertexArray mBuffer;
        };
    } // namespace sys
} // namespace stay

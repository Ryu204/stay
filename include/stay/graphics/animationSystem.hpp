#pragma once

#include "stay/ecs/system.hpp"

/*
    Render the whole scene by traversing every node using BFS
*/

namespace stay
{
    struct AnimationSystem
        : public ecs::UpdateSystem
        , public ecs::StartSystem
        , public ecs::InitSystem
        , public ecs::System
    {
            AnimationSystem(ecs::Manager* manager);
            void start() override;
            void update(float dt) override;
            void init(ecs::SystemContext& context) override;
        private:
            void setIndices(float dt);
            void setTextureRects();
            TextureManager* mTextures{nullptr};
    };
} // namespace stay

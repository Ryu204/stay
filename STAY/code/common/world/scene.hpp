#pragma once

#include "../ecs/manager.hpp"
#include "camera.hpp"
#include "../loader/sceneLoader.hpp"

namespace stay
{
    class Scene
    {
        public:
            Scene(std::filesystem::path&& filepath);
            ~Scene();
            void update(float dt);
            void start();
            void input(const sf::Event& event);
            void render(RTarget* target);
        private:
            void initialize();
            Camera mCamera;
            ecs::Manager mManager;
            Uptr<Node> mSceneRoot;
            SceneLoader mSceneLoader;

            const float mPixelsPerMeter;
    };
} // namespace stay

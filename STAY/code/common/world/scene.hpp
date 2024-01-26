#pragma once

#include "ecs/manager.hpp"
#include "camera.hpp"
#include "loader/sceneLoader.hpp"

namespace stay
{
    class Scene
    {
        public:
            Scene(std::filesystem::path&& filepath, RWin& window);
            ~Scene();
            static void update(float dt);
            static void start();
            void saveToFile();
            static void input(const sf::Event& event);
            void render(RTarget& target);
        private:
            RWin& mWindow;
            Camera mCamera;
            Uptr<Node> mSceneRoot;
            Uptr<SceneLoader> mSceneLoader;

            const float PIXELS_PER_METER;
    };
} // namespace stay

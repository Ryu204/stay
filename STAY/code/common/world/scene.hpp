#pragma once

#include "camera.hpp"
#include "loader/sceneLoader.hpp"
#include "type/texture.hpp"

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
            Serializable::Data getSystemConfig() const;

            RWin& mWindow;
            Camera mCamera;
            TextureManager mTextures;
            Uptr<Node> mSceneRoot;
            Uptr<SceneLoader> mSceneLoader;

            const float PIXELS_PER_METER;
            const std::string SYSTEMS_CONFIG_FILE;
    };
} // namespace stay

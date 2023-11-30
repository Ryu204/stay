#pragma once

#include <string>
#include "../utility/typedef.hpp"
#include "../type/serializable.hpp"
#include "../world/scene.hpp"

namespace stay
{
    namespace program
    {
        namespace detail
        {
            struct AppInfo : Serializable
            {
                std::size_t width;
                std::size_t height;
                std::string name;
                float updatesPerSec;
                Json::Value toJSONObject() const override;
                bool fetch(const Json::Value& data) override;
            };
        } // namespace detail
        
        class Application
        {
            public:
                static constexpr const char* INIT_FILE = "asset/init/init.json";

                Application();
                ~Application();
                void run();
            private:
                void input();
                void update(float dt);
                void render();
                void initialize();  
                void setUpRendering();  
                void saveProperties();

                detail::AppInfo mAppInfo;
                Uptr<RWin> mWindow;
                Uptr<Scene> mScene;

                Uptr<RTexture> mTexture;
                sf::VertexArray mSprite;
        };
    }
} // namespace stay
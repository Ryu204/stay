#pragma once

#include <string>
#include "stay/utility/typedef.hpp"
#include "stay/type/serializable.hpp"
#include "stay/world/scene.hpp"

namespace stay
{
    namespace program
    {        
        class Application
        {
            public:
                static constexpr const char* INIT_FILE = "asset/init.json";

                Application();
                ~Application();
                void run();
            private:
                struct Info : Serializable
                {
                    std::size_t width{500};
                    std::size_t height{500};
                    std::string name{"Unknown"};
                    float updatesPerSec{60.F};
                    SERIALIZE(width, height, name, updatesPerSec);
                };

                void input();
                static void update(float dt);
                void render();
                void initialize();  
                void setUpRendering();  
                void saveProperties();

                Info mAppInfo;
                Uptr<RWin> mWindow;
                Uptr<Scene> mScene;

                Uptr<RTexture> mTexture;
                sf::VertexArray mSprite;
        };
    }
} // namespace stay
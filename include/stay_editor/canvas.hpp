#pragma once

#include <SFML/Graphics.hpp>

#include <stay/world/camera.hpp>

namespace stay::editor
{
    class Canvas
    {
        public:
            void update();
            void clear();
            template<typename... Args>
            void draw(Args&&... args)
            {
                mTexture.draw(std::forward<Args>(args)...);
            }
            Camera& getCamera();
        private:
            unsigned int mWidth{0};
            unsigned int mHeight{0};
            sf::RenderTexture mTexture;
            Camera mCamera;
    };
}
#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "stay_editor/canvas.hpp"

namespace stay::editor
{
    class Editor
    {
        public:
            Editor();
            ~Editor();
            void run();
        private:
            void initialize();
            void input(sf::Time dt);
            void render();

            std::unique_ptr<sf::RenderWindow> mWindow;
            Canvas mGameCanvas;
    };
}
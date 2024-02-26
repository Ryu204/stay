#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace stay::editor
{
    class Editor
    {
        public:
            Editor();
            void run();
        private:
            void initialize();
            void input(sf::Time dt);
            void render();

            std::unique_ptr<sf::RenderWindow> mWindow;
    };
}
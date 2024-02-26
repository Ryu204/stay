#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

namespace stay::editor
{
    class Editor
    {
        public:
            Editor();
        private:
            void initialize();
            std::unique_ptr<sf::RenderWindow> window;
    };
}
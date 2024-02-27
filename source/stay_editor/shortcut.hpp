#pragma once

#include <SFML/Window.hpp>

namespace stay::editor 
{
    inline bool shortcutCtrl(sf::Keyboard::Scancode key)
    {
        return sf::Keyboard::isKeyPressed(key) && (
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LControl) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl)
        );
    }
}
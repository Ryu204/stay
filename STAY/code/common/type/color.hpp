#pragma once

#include <SFML/Graphics.hpp>

#include "serializable.hpp"

namespace stay
{
    struct Color : public sf::Color, public Serializable
    {
        using sf::Color::Color;
        Color(sf::Color color)
            : sf::Color(color)
        {}
        SERIALIZE(r, g, b, a);
    };
} // namespace stay

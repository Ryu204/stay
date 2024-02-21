#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
/*
    Utilities for interacting with SFML library
*/

namespace stay
{
    namespace utils
    {
        inline b2Color b2ColorFrom(const sf::Color& color)
        {
            return b2Color(color.r / 255.F, color.g / 255.F, color.b / 255.F, color.a / 255.F);
        }
    } // namespace utils
} // namespace stay

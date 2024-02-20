#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/b2_draw.h>

/*
    Utilities for interacting with SFML library
*/

namespace stay
{
    namespace utils
    {
        template <typename sfObject>
        void centerSf(sfObject& object)
        {
            auto bounds = object.getLocalBounds();
            object.setOrigin(bounds.left + bounds.width / 2.F, bounds.top + bounds.height / 2.F);
        }

        inline sf::Color sfColorFrom(const b2Color& color)
        {
            return sf::Color(255.F * color.r, 255.F * color.g, 255.F * color.b, 255.F * color.a);
        }
    } // namespace utils
} // namespace stay

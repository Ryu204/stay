#pragma once

#include <SFML/Graphics.hpp>

/*
    Utilities for interacting with SFML library
*/

namespace stay
{
    namespace utils
    {
        template <typename sfObject>
        void centersf(sfObject& object)
        {
            auto bounds = object.getLocalBounds();
            object.setOrigin(bounds.left + bounds.width / 2.F, bounds.top + bounds.height / 2.F);
        }
    } // namespace utils
} // namespace stay

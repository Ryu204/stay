#pragma once

#include <SFML/Graphics.hpp>

#include "../type/vector.hpp"
#include "../world/transform.hpp"

namespace stay
{
    namespace utils
    {
        inline sf::Vector2f vecTosfVec(Vector2 vec) // NOLINT
        {
            return {vec.x, vec.y};
        }
        inline sf::Transform transTosfTrans(const Transform& trans)
        {
            const auto& mat = trans.getMatrix();
            return { mat[0][0], mat[1][0], mat[3][0],
                     mat[0][1], mat[1][1], mat[3][1],
                     mat[0][3], mat[1][3], mat[3][3] };
        }
    } // namespace utils
} // namespace stay

#pragma once

#include <SFML/Graphics.hpp>

#include "stay/world/transform.hpp"

namespace stay
{
    namespace utils
    {
        // @brief Convert vector2 of type `in` to `out`
        template <typename out, typename in>
        out convertVec2(const in& vec2)
        {
            return out(vec2.x, vec2.y);
        }

        // @brief Convert vector3 of type `in` to `out`
        template <typename out, typename in>
        out convertVec3(const in& vec3)
        {
            return out(vec3.x, vec3.y, vec3.z);
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

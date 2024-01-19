#pragma once

#include <type_traits>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "serializable.hpp"

namespace stay
{
    class Vector2Int : public glm::vec<2, int>, public Serializable
    {
        public:
            using glm::vec<2, int>::vec;
            Vector2Int(const glm::vec<2, int>& other)
                : glm::vec<2, int>{other}
            {}
            SERIALIZE(x, y)
    };

    class Vector3Int : public glm::vec<3, int>, public Serializable
    {
        public:
            using glm::vec<3, int>::vec;
            Vector3Int(const glm::vec<3, int>& other)
                : glm::vec<3, int>{other}
            {}
            SERIALIZE(x, y, z)
    };

    class Vector2 : public glm::vec2, public Serializable
    {
        public:
            using glm::vec2::vec;
            Vector2(const glm::vec2& vec)
                : glm::vec2(vec)
            {}
            SERIALIZE(x, y)
    };

    class Vector3 : public glm::vec3, public Serializable
    {       
        public:
            using glm::vec3::vec;
            Vector3(const glm::vec3& vec)
                : glm::vec3(vec)
            { }
            Vector3(const glm::vec2& vec)
                : glm::vec3(vec.x, vec.y, 0.F)
            { }
            SERIALIZE(x, y, z)
    };
    /*        Y+
              |
              |     SCREEN
              |
              |
              /------------------ X+
            /
          /
        /      
      Z- <-- (PLAYER)
    */
    inline constexpr Vector3 vectorUp(0.F, 1.F, 0.F);
    inline constexpr Vector3 vectorDown(0.F, -1.F, 0.F);
    inline constexpr Vector3 vectorLeft(-1.F, 0.F, 0.F);
    inline constexpr Vector3 vectorRight(1.F, 0.F, 0.F);
    inline constexpr Vector3 vectorBack(0.F, 0.F, -1.F);
    inline constexpr Vector3 vectorForward(0.F, 0.F, 1.F);
} // namespace stay

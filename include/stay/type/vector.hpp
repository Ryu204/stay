#pragma once

#include "serializable.hpp"
#include "stay/utility/crtpHelper.hpp"

#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace stay
{
    namespace detail
    {
        template <typename T>
        struct Vec2 
        {
            CRTP_UNDERLYING(T)
            float getLength() const 
            {
                const auto& t = underlying();
                return std::sqrt(t.x * t.x + t.y * t.y);
            }
            T norm() const
            {
                const auto l = getLength();
                assert(l != 0.F && "attempt to normalize a zero vector");
                return underlying() / l;
            }
            template <typename out>
            out toVec2() const 
            {
                const auto& t = underlying();
                return out{ t.x, t.y };
            }
            template <typename out>
            static T from(const out& t)
            {
                return T{ t.x, t.y };
            }
            template <typename out>
            out toVec3() const 
            {
                const auto& t = underlying();
                return out{ t.x, t.y, 0.F };
            }
        };

        template <typename T>
        struct Vec3
        {
            CRTP_UNDERLYING(T)
            float getLength() const 
            {
                const auto& t = underlying();
                return std::sqrt(t.x * t.x + t.y * t.y + t.z * t.z);
            }
            T norm() const
            {
                const auto l = getLength();
                assert(l != 0.F && "attempt to normalize a zero vector");
                return underlying() / l;
            }
            template <typename out>
            static T from(const out& t)
            {
                return T{ t.x, t.y, t.z };
            }
            template <typename out>
            out toVec2() const 
            {
                const auto& t = underlying();
                return out{ t.x, t.y };
            }
            template <typename out>
            out toVec3() const 
            {
                const auto& t = underlying();
                return out{ t.x, t.y,  t.z };
            }
        };
    } // namespace detail
    class Vector2Int : public glm::vec<2, int>, public Serializable, public detail::Vec2<Vector2Int>
    {
        public:
            using glm::vec<2, int>::vec;
            Vector2Int(const glm::vec<2, int>& other)
                : glm::vec<2, int>{other}
            {}
            SERIALIZE(x, y)
    };

    class Vector3Int : public glm::vec<3, int>, public Serializable, public detail::Vec3<Vector3Int>
    {
        public:
            using glm::vec<3, int>::vec;
            Vector3Int(const glm::vec<3, int>& other)
                : glm::vec<3, int>{other}
            {}
            SERIALIZE(x, y, z)
    };

    class Vector2 : public glm::vec2, public Serializable, public detail::Vec2<Vector2>
    {
        public:
            using glm::vec2::vec;
            Vector2(const glm::vec2& vec)
                : glm::vec2(vec)
            {}
            SERIALIZE(x, y)
    };

    class Vector3 : public glm::vec3, public Serializable, public detail::Vec3<Vector3>
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

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
            Json::Value toJSONObject() const override
            {
                Json::Value res(Json::arrayValue);
                res.append(x);
                res.append(y);
                return res;
            }
            bool fetch(const Json::Value& data) override
            {
                if (data.isArray() && data.size() >= 2 && data[0u].isNumeric() && data[1u].isNumeric()) 
                {
                    x = data[0u].asInt();
                    y = data[1u].asInt();
                    return true;
                }   
                return false;
            }
    };

    class Vector3Int : public glm::vec<3, int>, public Serializable
    {
        public:
            using glm::vec<3, int>::vec;
            Json::Value toJSONObject() const override
            {
                Json::Value res(Json::arrayValue);
                res.append(x);
                res.append(y);
                res.append(z);
                return res;
            }
            bool fetch(const Json::Value& data) override
            {
                if (data.isArray() && data.size() >= 3 && data[0u].isNumeric() && data[1u].isNumeric() && data[2u].isNumeric()) 
                {
                    x = data[0u].asInt();
                    y = data[1u].asInt();
                    z = data[2u].asInt();
                    return true;
                }
                return false;
            }
    };

    class Vector2 : public glm::vec2, public Serializable
    {
        public:
            using glm::vec2::vec;
            Vector2(const glm::vec2& vec)
                : glm::vec2(vec)
            {}
            Json::Value toJSONObject() const override
            {
                Json::Value res(Json::arrayValue);
                res.append(x);
                res.append(y);
                return res;
            }

            bool fetch(const Json::Value& data) override
            {
                if (data.isArray() && data.size() >= 2 && data[0u].isNumeric() && data[1u].isNumeric()) 
                {
                    x = data[0u].asFloat();
                    y = data[1u].asFloat();
                    return true;
                }   
                return false;
            }
    };

    class Vector3 : public glm::vec3, public Serializable
    {       
        public:
            using glm::vec3::vec;
            Vector3(const glm::vec3& vec)
                : glm::vec3(vec)
            { }
            Json::Value toJSONObject() const override
            {
                Json::Value res(Json::arrayValue);
                res.append(x);
                res.append(y);
                res.append(z);
                return res;
            }
            bool fetch(const Json::Value& data) override
            {
                if (data.isArray() && data.size() >= 3 && data[0u].isNumeric() && data[1u].isNumeric() && data[2u].isNumeric()) 
                {
                    x = data[0u].asFloat();
                    y = data[1u].asFloat();
                    z = data[2u].asFloat();
                    return true;
                }
                return false;
            }
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

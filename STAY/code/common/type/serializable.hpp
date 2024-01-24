#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include "utility/foreach.hpp"
#include "utility/typedef.hpp"
#include "ecs/type.hpp"

namespace stay
{
    class Serializable 
    {
        public:
            using Data = nlohmann::json;
            virtual Data toJSONObject() const = 0;
            virtual bool fetch(const Data& /*data*/) = 0;
    };

    template <typename T, whereIs(T, Serializable)>
    Serializable::Data toJSON(const T& t)
    {
        return t.toJSONObject();
    }
    template <typename T, whereIs(T, Serializable)>
    bool fromJSON(T& t, const Serializable::Data& data)
    {
        return t.fetch(data);
    }
    Serializable::Data toJSON(const ecs::Entity& t);
    bool fromJSON(ecs::Entity& t, const Serializable::Data& data);
    Serializable::Data toJSON(const int& t);
    bool fromJSON(int& t, const Serializable::Data& data);
    Serializable::Data toJSON(const unsigned short& t);
    bool fromJSON(unsigned short& t, const Serializable::Data& data);
    Serializable::Data toJSON(const unsigned char& t);
    bool fromJSON(unsigned char& t, const Serializable::Data& data);
    Serializable::Data toJSON(const std::size_t& t);
    bool fromJSON(std::size_t& t, const Serializable::Data& data);
    Serializable::Data toJSON(const float& t);
    bool fromJSON(float& t, const Serializable::Data& data);
    Serializable::Data toJSON(const bool& t);
    bool fromJSON(bool& t, const Serializable::Data& data);
    Serializable::Data toJSON(const std::string& t);
    bool fromJSON(std::string& t, const Serializable::Data& data);
    template <typename T>
    inline Serializable::Data toJSON(const std::vector<T>& t)
    {
        Serializable::Data res;
        for (const auto& i : t)
            res.emplace_back(toJSON(i));
        return res;
    }
    template <typename T>
    inline bool fromJSON(std::vector<T>& t, const Serializable::Data& data)
    {
        if (!data.is_array())
            return false;
        t.clear();
        for (const auto& val : data)
        {
            t.push_back(T());
            if (!t.back().fetch(val))
                return false;
        }
        return true;
    }
} // namespace stay
#define stay_GET_JSON(x) { res[#x] = stay::toJSON(x); }
#define stay_FETCH_JSON(x) { if (!stay::fromJSON(x, val[#x])) return false; }

// @example 
// ```cpp
// struct Foo : public Serializable
// {
//     Vector2 vec;
//     Color color;
//     SERIALIZE(vec, color)
// }
// ```
#define SERIALIZE(...) \
    Serializable::Data toJSONObject() const override {\
        Serializable::Data res;\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool fetch(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        return true;\
    }

// @example 
// ```cpp
// struct Foo : public Serializable
// {
//     Vector2 vec;
//     Color color;
//     bool cacheChanged;
//     SERIALIZE_POSTPROCESSING(vec, color)
//     void postSerialization()
//     {
//          cacheChanged = true;
//     }
// }
// ```
#define SERIALIZE_POSTPROCESSING(...) \
    Serializable::Data toJSONObject() const override {\
        Serializable::Data res;\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool fetch(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        postSerialization();\
        return true;\
    }

#define SERIALIZE_PREPROCESSING(...) \
    Serializable::Data toJSONObject() const override {\
        Serializable::Data res;\
        preSerialization();\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool fetch(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        return true;\
    }

#define SERIALIZE_PROCESSING(...) \
    Serializable::Data toJSONObject() const override {\
        Serializable::Data res;\
        preSerialization();\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool fetch(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        postSerialization();\
        return true;\
    }

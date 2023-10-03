#pragma once

#include <cassert>

#include <json/json.h>

#include "../utility/foreach.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    class Serializable 
    {
        public:
            virtual Json::Value toJSONObject() const = 0;
            virtual bool fetch(const Json::Value& /*data*/) = 0;
    };

    template <typename T>
    Json::Value toJSON(const T& t)
    {
        if constexpr (std::is_base_of_v<Serializable, T>)
            return t.toJSONObject();
        assert(true && "Must implement specialization");
    }
    template <typename T>
    bool fromJSON(T& t, const Json::Value& data)
    {
        if constexpr (std::is_base_of_v<Serializable, T>)
            return t.fetch(data);
        assert(true && "Must implement specialization");
    }
    template <>
    Json::Value toJSON<int>(const int& t);
    template <>
    bool fromJSON<int>(int& t, const Json::Value& data);
    template <>
    Json::Value toJSON<float>(const float& t);
    template <>
    bool fromJSON<float>(float& t, const Json::Value& data);
    template <>
    Json::Value toJSON<bool>(const bool& t);
    template <>
    bool fromJSON<bool>(bool& t, const Json::Value& data);
    template <>
    Json::Value toJSON<std::string>(const std::string& t);
    template <>
    bool fromJSON<std::string>(std::string& t, const Json::Value& data);
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
    Json::Value toJSONObject() const override {\
        Json::Value res;\
        FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool fetch(const Json::Value& val) override {\
        FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
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
    Json::Value toJSONObject() const override {\
        Json::Value res;\
        FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool fetch(const Json::Value& val) override {\
        FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        postSerialization();\
        return true;\
    }

#pragma once

#include <json/json.h>
#include "../utility/foreach.hpp"

namespace stay
{
    class Serializable 
    {
        public:
            virtual Json::Value toJSONObject() const = 0;
            virtual bool fetch(const Json::Value& /*data*/) = 0;
    };
} // namespace stay

#define stay_GET_JSON(x) { res[#x] = x.toJSONObject(); }
#define stay_FETCH_JSON(x) { if (!x.fetch(val[#x])) return false; }

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

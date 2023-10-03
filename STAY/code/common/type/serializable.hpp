#pragma once

#include <cassert>
#include <vector>

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

    template <typename T, whereIs(T, Serializable)>
    Json::Value toJSON(const T& t)
    {
        return t.toJSONObject();
    }
    template <typename T, whereIs(T, Serializable)>
    bool fromJSON(T& t, const Json::Value& data)
    {
        return t.fetch(data);
    }
    Json::Value toJSON(const int& t);
    bool fromJSON(int& t, const Json::Value& data);
    Json::Value toJSON(const float& t);
    bool fromJSON(float& t, const Json::Value& data);
    Json::Value toJSON(const bool& t);
    bool fromJSON(bool& t, const Json::Value& data);
    Json::Value toJSON(const std::string& t);
    bool fromJSON(std::string& t, const Json::Value& data);
    template <typename T>
    inline Json::Value toJSON(const std::vector<T>& t)
    {
        Json::Value res(Json::arrayValue);
        for (const auto& i : t)
            res.append(toJSON(i));
        return res;
    }
    template <typename T>
    inline bool fromJSON(std::vector<T>& t, const Json::Value& data)
    {
        if (!data.isArray())
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

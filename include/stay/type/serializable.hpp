#pragma once

#include <vector>
#include <optional>

#include <nlohmann/json.hpp>

#include "stay/utility/foreach.hpp"
#include "stay/utility/typedef.hpp"
#include "stay/ecs/type.hpp"

namespace stay
{
    class Serializable 
    {
        public:
            using Data = nlohmann::json;
            virtual Data serialize() const = 0;
            virtual bool deserialization(const Data& /*data*/) = 0;

            constexpr Serializable() = default;
            virtual ~Serializable() = default;
    };

    template <typename T, whereIs(T, Serializable)>
    Serializable::Data toJSON(const T& t)
    {
        return t.serialize();
    }
    template <typename T, whereIs(T, Serializable)>
    bool fromJSON(T& t, const Serializable::Data& data)
    {
        return t.deserialization(data);
    }
    // Basic types
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

    // Container types
    template <typename T>
    Serializable::Data toJSON(const std::vector<T>& t)
    {
        Serializable::Data res;
        for (const auto& i : t)
            res.emplace_back(toJSON(i));
        return res;
    }
    template <typename T>
    bool fromJSON(std::vector<T>& t, const Serializable::Data& data)
    {
        if (!data.is_array())
            return false;
        t.clear();
        for (const auto& val : data)
        {
            t.push_back(T());
            if (!t.back().deserialization(val))
                return false;
        }
        return true;
    }
    template <typename T>
    Serializable::Data toJSON(const std::optional<T>& t)
    {
        if (!t.has_value())
            return "none";
        return toJSON(t.value());
    }
    template <typename T>
    bool fromJSON(std::optional<T>& t, const Serializable::Data& data)
    {
        if (data.is_string() && data.get<std::string>() == "none")
        {
            t.reset();
            return true;
        }
        std::make_optional<T>().swap(t);
        return fromJSON(t.value(), data);
    }
    template <typename T>
    Serializable::Data toJSON(const std::unordered_map<std::string, T>& m)
    {
        Serializable::Data res;
        for (const auto& [name, val] : m) 
            res[name] = toJSON(val);
        return res;
    }
    template <typename T>
    bool fromJSON(std::unordered_map<std::string, T>& t, const Serializable::Data& data)
    {
        for (const auto& itr : data.items()) 
        {
            auto& val = t[itr.key()];
            if (!fromJSON(val, itr.value()))
                return false;
        }
        return true;
    }
} // namespace stay
#define stay_GET_JSON(x) { res[#x] = stay::toJSON(x); }
#define stay_FETCH_JSON(x) { if (!val.contains(#x) || !stay::fromJSON(x, val[#x])) return false; }

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
    Serializable::Data serialize() const override {\
        Serializable::Data res;\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool deserialization(const Serializable::Data& val) override {\
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
//     DESERIALIZE_POSTPROCESSING(vec, color)
//     void postDeserialization()
//     {
//          cacheChanged = true;
//     }
// }
// ```
#define DESERIALIZE_POSTPROCESSING(...) \
    Serializable::Data serialize() const override {\
        Serializable::Data res;\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool deserialization(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        postDeserialization();\
        return true;\
    }

#define SERIALIZE_PREPROCESSING(...) \
    Serializable::Data serialize() const override {\
        Serializable::Data res;\
        preSerialization();\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool deserialization(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        return true;\
    }

#define SERIALIZE_PROCESSING(...) \
    Serializable::Data serialize() const override {\
        Serializable::Data res;\
        preSerialization();\
        stay_FOR_EACH(stay_GET_JSON, __VA_ARGS__)\
        return res;\
    }\
    bool deserialization(const Serializable::Data& val) override {\
        stay_FOR_EACH(stay_FETCH_JSON, __VA_ARGS__)\
        postDeserialization();\
        return true;\
    }

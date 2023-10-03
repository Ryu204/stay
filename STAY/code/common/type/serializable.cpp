#include "serializable.hpp"

namespace stay
{
    template <>
    Json::Value toJSON<int>(const int& t)
    {
        return Json::Value(t);
    }
    template <>
    bool fromJSON<int>(int& t, const Json::Value& data)
    {
        if (data.isInt())
        {
            t = data.asInt();
            return true;
        }
        return false;
    }
    template <>
    Json::Value toJSON<float>(const float& t)
    {
        return Json::Value(t);
    }
    template <>
    bool fromJSON<float>(float& t, const Json::Value& data)
    {
        if (data.isNumeric())
        {
            t = data.asFloat();
            return true;
        }
        return false;
    }
    template <>
    Json::Value toJSON<bool>(const bool& t)
    {
        return Json::Value(t);
    }
    template <>
    bool fromJSON<bool>(bool& t, const Json::Value& data)
    {
        if (data.isBool())
        {
            t = data.asBool();
            return true;
        }
        return false;
    }
    template <>
    Json::Value toJSON<std::string>(const std::string& t)
    {
        return Json::Value(t);
    }
    template <>
    bool fromJSON<std::string>(std::string& t, const Json::Value& data)
    {
        if (data.isString())
        {
            data.asString().swap(t);
            return true;
        }
        return false;
    }
} // namespace stay
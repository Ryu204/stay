#include "serializable.hpp"

namespace stay
{
    Json::Value toJSON(const int& t)
    {
        return Json::Value(t);
    }
    bool fromJSON(int& t, const Json::Value& data)
    {
        if (data.isInt())
        {
            t = data.asInt();
            return true;
        }
        return false;
    }
    Json::Value toJSON(const float& t)
    {
        return Json::Value(t);
    }
    bool fromJSON(float& t, const Json::Value& data)
    {
        if (data.isNumeric())
        {
            t = data.asFloat();
            return true;
        }
        return false;
    }
    Json::Value toJSON(const bool& t)
    {
        return Json::Value(t);
    }
    bool fromJSON(bool& t, const Json::Value& data)
    {
        if (data.isBool())
        {
            t = data.asBool();
            return true;
        }
        return false;
    }
    Json::Value toJSON(const std::string& t)
    {
        return Json::Value(t);
    }
    bool fromJSON(std::string& t, const Json::Value& data)
    {
        if (data.isString())
        {
            data.asString().swap(t);
            return true;
        }
        return false;
    }
} // namespace stay
#include "stay/type/serializable.hpp"

namespace stay
{
    Serializable::Data toJSON(const ecs::Entity& t)
    {
        return Serializable::Data(static_cast<int>(t));
    }
    bool fromJSON(ecs::Entity& t, const Serializable::Data& data)
    {
        if (data.is_number_integer())
        {
            t = static_cast<ecs::Entity>(data);
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const int& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(int& t, const Serializable::Data& data)
    {
        if (data.is_number_integer())
        {
            t = data;
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const std::size_t& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(unsigned short& t, const Serializable::Data& data)
    {
        if (data.is_number_integer())
        {
            t = data;
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const unsigned short& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(unsigned char& t, const Serializable::Data& data)
    {
        if (data.is_number_integer())
        {
            t = data;
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const unsigned char& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(std::size_t& t, const Serializable::Data& data)
    {
        if (data.is_number_integer())
        {
            t = data;
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const float& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(float& t, const Serializable::Data& data)
    {
        if (data.is_number_float())
        {
            t = data;
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const bool& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(bool& t, const Serializable::Data& data)
    {
        if (data.is_boolean())
        {
            t = data;
            return true;
        }
        return false;
    }
    Serializable::Data toJSON(const std::string& t)
    {
        return Serializable::Data(t);
    }
    bool fromJSON(std::string& t, const Serializable::Data& data)
    {
        if (data.is_string())
        {
            t = data;
            return true;
        }
        return false;
    }
} // namespace stay
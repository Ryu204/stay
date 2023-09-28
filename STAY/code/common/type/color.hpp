#pragma once

#include <SFML/Graphics.hpp>

#include "serializable.hpp"

namespace stay
{
    struct Color : public sf::Color, public Serializable
    {
        using sf::Color::Color;
        Color(sf::Color color)
            : sf::Color(color)
        {}
        Json::Value toJSONObject() const override
        {
            Json::Value res(Json::ValueType::arrayValue);
            res.append(r);
            res.append(g);
            res.append(b);
            res.append(a);
            return res;
        }
        bool fetch(const Json::Value& data) override
        {
            bool invalid = data.type() != Json::arrayValue || data.size() != 4;
            if (invalid)
            {
                *(static_cast<sf::Color*>(this)) = sf::Color::White;
                return false;
            }
            r = data[0].asInt();
            g = data[1].asInt();
            b = data[2].asInt();
            a = data[3].asInt();
            return true;
        }
    };
} // namespace stay

#pragma once

#include "type/serializable.hpp"
#include "type/rect.hpp"

namespace stay
{   
    struct TextureInfo : Serializable
    {
        TextureInfo(std::string id = "none", Rect rect = Rect{})
            : id{std::move(id)}
            , rect{std::move(rect)}
        {}
        std::string id;
        Rect rect;
        SERIALIZE(id, rect);
    };
} // namespace stay
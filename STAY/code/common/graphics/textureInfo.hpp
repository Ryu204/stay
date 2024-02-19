#pragma once

#include <cstddef>

#include "type/serializable.hpp"
#include "type/rect.hpp"

namespace stay
{   
    struct TextureInfo : Serializable
    {
        TextureInfo(std::string id = "none", Rect rect = Rect{}, Vector2 pivot = Vector2{0.5F, 0.5F})
            : id{std::move(id)}
            , rect{std::move(rect)}
            , pivot{std::move(pivot)}
        {}
        std::string id;
        Rect rect;
        Vector2 pivot;
        SERIALIZE(id, rect, pivot);
    };
} // namespace stay
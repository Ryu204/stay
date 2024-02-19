#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "textureInfo.hpp"
#include "type/color.hpp"
#include "ecs/component.hpp"

namespace stay
{
    struct Render : public ecs::Component
    {
        Render(Color color = Color(), Vector2 size = Vector2(), int zOrder = 0)
            : color{std::move(color)}
            , size{std::move(size)}
            , zOrder{zOrder}
        {}
        Render(Color color, Vector2 size, int zOrder, const std::string& textureId, const Rect& textureRect, const Vector2& pivot = Vector2{})
            : color{std::move(color)}
            , size{std::move(size)}
            , zOrder{zOrder}
            , textureInfo{std::make_optional<TextureInfo>(textureId, textureRect, pivot)}
        {}
        Render(Color color, Vector2 size, int zOrder, TextureInfo info)
            : color{std::move(color)}
            , size{std::move(size)}
            , zOrder{zOrder}
            , textureInfo{std::make_optional<TextureInfo>(std::move(info))}
        {}
        
        Color color;
        Vector2 size;
        int zOrder;
        std::optional<TextureInfo> textureInfo;
        COMPONENT(Render, color, size, textureInfo);
    };
} // namespace stay
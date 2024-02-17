#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "ecs/component.hpp"
#include "type/color.hpp"
#include "type/rect.hpp"

namespace stay
{
    struct Render : public ecs::Component
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

        Render(Color color = Color(), Vector2 size = Vector2())
            : color{std::move(color)}
            , size{std::move(size)}
        {}
        Render(Color color, Vector2 size, const std::string& textureId, const Rect& textureRect)
            : color{std::move(color)}
            , size{std::move(size)}
            , textureInfo{std::make_optional<TextureInfo>(textureId, textureRect)}
        {}
        Color color;
        Vector2 size;
        std::optional<TextureInfo> textureInfo;
        COMPONENT(Render, color, size, textureInfo);
    };
} // namespace stay
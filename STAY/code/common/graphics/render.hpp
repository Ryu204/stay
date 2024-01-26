#pragma once

#include <SFML/Graphics.hpp>

#include "ecs/component.hpp"
#include "type/color.hpp"

namespace stay
{
    struct Render : public ecs::Component
    {
        Render(const Color& color = Color(), const Vector2& size = Vector2())
        {
            this->color = color;
            this->size = size;
        }
        Color color;
        Vector2 size;
        COMPONENT(Render, color, size);
    };
} // namespace stay
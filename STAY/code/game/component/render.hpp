#pragma once

#include <SFML/Graphics.hpp>

#include "ecs/component.hpp"
#include "type/color.hpp"
#include "type/serializable.hpp"

namespace stay
{
    namespace comp
    {
        struct Render : public ecs::Component<Render>
        {
            Render(const Color& color = Color(), const Vector2& size = Vector2())
            {
                this->color = color;
                this->size = size;
            }
            Color color;
            Vector2 size;
            COMPONENT(render, color, size);
        };
    } // namespace comp
} // namespace stay
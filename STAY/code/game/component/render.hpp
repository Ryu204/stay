#pragma once

#include <SFML/Graphics.hpp>

#include "../../common/ecs/component.hpp"
#include "../../common/type/color.hpp"
#include "../../common/loader/componentLoader.hpp"
#include "../../common/utility/convert.hpp"
#include "../../common/type/serializable.hpp"

namespace stay
{
    namespace comp
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
            SERIALIZE(color, size);
        };
    } // namespace comp
} // namespace stay
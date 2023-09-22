#pragma once

#include <SFML/Graphics.hpp>

#include "../../common/ecs/component.hpp"

namespace stay
{
    namespace comp
    {
        struct Render : public ecs::Component
        {
            Render(const sf::Color& color, const sf::Vector2f& size)
            {
                this->color = color;
                this->size = size;
            }
            sf::Color color;
            sf::Vector2f size;
        };
    } // namespace comp
} // namespace stay
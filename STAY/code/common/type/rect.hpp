#pragma once

#include <SFML/Graphics.hpp>

#include "serializable.hpp"

namespace stay
{
    namespace detail
    {
        template <typename T>
        struct Rect : public sf::Rect<T>, public Serializable
        {
            using sf::Rect<T>::Rect;
            Rect(const sf::Rect<T>& other)
                : sf::Rect<T>{other}
            {}
            Rect(sf::Rect<T>&& other)
                : sf::Rect<T>{std::move(other)}
            {}
            SERIALIZE(this->top, this->left, this->width, this->height);
        };
    }
    using Rect = detail::Rect<float>;
    using RectInt = detail::Rect<int>;
} // namespace stay

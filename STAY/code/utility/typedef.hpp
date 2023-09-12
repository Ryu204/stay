#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace stay
{
    using RWin = sf::RenderWindow;
    template <typename T>
    using SPtr = std::shared_ptr<T>;
    template<typename T>
    using Uptr = std::unique_ptr<T>;
} // namespace stay

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "define.hpp"

namespace stay
{
    using RWin = sf::RenderWindow;
    using RTarget = sf::RenderTarget;
    using RTexture = sf::RenderTexture;
    using RStates = sf::RenderStates;
    template <typename T>
    using SPtr = std::shared_ptr<T>;
    template<typename T>
    using Uptr = std::unique_ptr<T>;
} // namespace stay

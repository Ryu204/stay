#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

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
    #define RAD2DEG 57.29578F
    #define DEG2RAD 0.0174532925F
} // namespace stay

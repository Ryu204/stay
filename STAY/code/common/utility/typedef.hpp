#pragma once

#include <memory>
#include <type_traits>

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

    // I'm a cursed C++ programmer for mimicking C#
    #define whereIs(Derived, Base) std::enable_if_t<std::is_base_of_v<Base, Derived>, bool> = true
} // namespace stay

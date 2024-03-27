#pragma once

#include <limits>

#include "stay/ecs/component.hpp"
#include "stay/type/rect.hpp"

namespace stay
{
    struct CameraController : ecs::Component
    {
        CameraController(float height = 5.F, Rect bounds = Rect{
            Vector2{-1e9, -1e9},
            Vector2{1e9, 1e9}
        })
            : height{height}
            , bounds{std::move(bounds)}
        {}
        float height;
        Rect bounds;
        COMPONENT(CameraController, height, bounds)
    };
} // namespace stay
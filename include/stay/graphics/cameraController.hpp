#pragma once

#include "stay/ecs/component.hpp"

namespace stay
{
    struct CameraController : ecs::Component
    {
        CameraController(float height = 5.F)
            : height{height}
        {}
        float height;
        COMPONENT(CameraController, height)
    };
} // namespace stay
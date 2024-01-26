#pragma once

#include "utility/typedef.hpp"

namespace stay 
{
    class Camera;

    namespace ecs
    {
        struct SystemContext
        {
            SystemContext(Camera& camera, RWin& window)
                : camera(camera)
                , window(window)
            {}
            Camera& camera;
            RWin& window;
        };
    } // namespace ecs
} // namespace stay
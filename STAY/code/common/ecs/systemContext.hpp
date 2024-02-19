#pragma once

#include "utility/typedef.hpp"
#include "type/texture.hpp"

namespace stay 
{
    class Camera;

    namespace ecs
    {
        struct SystemContext
        {
            SystemContext(Camera& camera, RWin& window, TextureManager& textures)
                : camera{camera}
                , window{window}
                , textures{textures}
            {}
            Camera& camera;
            RWin& window;
            TextureManager& textures;
        };
    } // namespace ecs
} // namespace stay
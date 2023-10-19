#pragma once

#include <box2d/box2d.h>

#include "../utility/singleton.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    namespace phys
    {
        namespace details
        {
            struct WorldTrait
            {
                using Type = b2World;
                static Uptr<b2World> create();
                static void shutdown(Uptr<b2World>& world);
            };
        } // namespace details
        
        using World = utils::Singleton<details::WorldTrait>;
    } // namespace phys
} // namespace stay

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
                static Uptr<b2World> create()
                {
                    return std::make_unique<b2World>(b2Vec2{0.F, -20.F});
                }
                static void shutdown(Uptr<b2World>& world)
                {
                    world.reset();
                }
            };
        } // namespace details
        
        using World = utils::Singleton<details::WorldTrait>;
    } // namespace phys
} // namespace stay

#pragma once

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/box2d.h>

#include "type/vector.hpp"
#include "utility/convert.hpp"
#include "utility/singleton.hpp"
#include "utility/typedef.hpp"

namespace stay
{
    namespace phys
    {
        namespace details
        {
            struct WorldWrapper : b2World
            {
                WorldWrapper(const Vector2& gravity)
                    : b2World(utils::convertVec2<b2Vec2>(gravity))
                {}
            };
        } // namespace details
        
        using World = utils::Singleton<details::WorldWrapper>;
    } // namespace phys
} // namespace stay

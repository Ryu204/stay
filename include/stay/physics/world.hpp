#pragma once

#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/box2d.h>

#include "stay/type/vector.hpp"
#include "stay/utility/convert.hpp"
#include "stay/utility/singleton.hpp"
#include "stay/utility/typedef.hpp"

namespace stay
{
    namespace phys
    {
        namespace details
        {
            struct WorldWrapper : b2World
            {
                WorldWrapper(const Vector2& gravity)
                    : b2World(gravity.toVec2<b2Vec2>())
                {}
            };
        } // namespace details
        
        using World = utils::Singleton<details::WorldWrapper>;
    } // namespace phys
} // namespace stay

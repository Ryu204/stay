#pragma once

#include <box2d/box2d.h>

#include "../type/vector.hpp"

namespace stay
{
    namespace phys
    {
        class Collider;
        struct Collision
        {
            Collision(b2Contact* contact, bool isA);
            Collider* other;
            Vector2 normal;
            Vector2 position;
        };
    } // namespace phys
} // namespace stay

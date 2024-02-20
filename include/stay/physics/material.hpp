#pragma once

#include <box2d/box2d.h>

#include "stay/type/serializable.hpp"

namespace stay
{
    namespace phys
    {
        class Collider;
        class Rigidbody;
        class Material : public Serializable, public b2FixtureDef
        {
            public:
                Material(float density = 1.F, float friction = 0.F, float restituition = 0.F)
                {
                    this->density = density;
                    this->friction = friction;
                    this->restitution = restituition;
                }
                Material(const b2FixtureDef& other)
                    : b2FixtureDef{other}
                {}
                using b2FixtureDef::b2FixtureDef;
                SERIALIZE(density, friction, restitution, filter.categoryBits, filter.maskBits)
        };
    } // namespace sys
} // namespace stay
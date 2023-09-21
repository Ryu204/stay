#pragma once

#include <box2d/box2d.h>

#include "../utility/convert.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    namespace phys
    {
        enum class BodyType
        {
            STATIC, // Move under explicit `setPosition(...)`, only collides DYNAMIC
            KINEMATIC, // Move under`setVelocity(...)`, only collides DYNAMIC
            DYNAMIC // Move completely by internal solver
        };
        class Collider;
        // @brief An abstract component, only functions if a collider is presented. Manipulate velocity and force.
        class RigidBody
        {
            public:
                // @param `angle` angle from OY+ to object's local OY+ in degree, positive if the angle is anti-clockwise
                RigidBody(b2World* world, const Vector2& position = Vector2(10.F, -10.F), float angle = 0.F, BodyType type = BodyType::STATIC);
                ~RigidBody();
                void setPosition(const Vector2& position);
                Vector2 getPosition() const;
                // @param `angle` the angle between OY+ and object's local OY+ in degree, positive if anti-clockwise
                void setAngle(float angle);
                float getAngle() const;
                void applyForce(const Vector2& force);
                void setVelocity(const Vector2& velocity);
                Vector2 getVelocity() const;
                // @ param `velocity` the angular velocity between OY+ and object's local OY+ in degree, positive if rotating clockwise
                void setAngularVelocity(float velocity);
                float getAngularVelocity() const;
                void setFixedRotation(bool fixed);
                void setGravityScale(float scale);
                b2Fixture* attachFixture(const b2FixtureDef& properties);
            private:
                b2World* mWorld;
                b2Body* mBody;
        };
    } // namespace phys
} // namespace stay

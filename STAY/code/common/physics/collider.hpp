#pragma once

#include <variant>

#include <SFML/System/NonCopyable.hpp>
#include <box2d/box2d.h>

#include "../type/vector.hpp"
#include "../utility/convert.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    namespace phys
    {
        class Material;
        class RigidBody;
        class Collider : private sf::NonCopyable
        {
            public:
                struct Box {
                    Vector2 position;
                    Vector2 halfSize;
                    float angle; // in game coords
                };
                struct Circle {
                    Vector2 position;
                    float radius;
                };
                using Info = std::variant<Box, Circle>;
                
                // @note Only `body` needs to stay alive after the construction
                Collider(const Info& info, RigidBody* body, const Material* mat);  
                virtual ~Collider();
                void setMaterial(const Material* mat);
                void setTrigger(bool isTrigger);
                bool getTrigger() const;
            private:
                void attachToRigidBody(const Info& info, RigidBody* body, const Material* mat);
                // @warning This function uses `new` keyword to allocate new shape, care to their destruction must be taken
                static b2Shape* createShape(const Collider::Info& info);

                b2Fixture* mFixture;
                friend class Material;
        };

        class Material
        {
            public:
                Material(float density, float friction, float restituition);
                void setDensity(float density);
                void setFriction(float friction);
                void setRestituition(float restituition);
                b2FixtureDef getFixtureDef() const;
                
            private:
                void updateCollider(Collider* collider) const;
                b2FixtureDef mDef;
                friend class Collider;
        };
    } // namespace phys
} // namespace stay
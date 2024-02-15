#include "collision.hpp"

namespace stay
{
    namespace phys
    {
        Collision::Collision(b2Contact* contact, bool isA)
        {
            other = isA ? reinterpret_cast<Collider*>(contact->GetFixtureB()->GetUserData().pointer)
                        : reinterpret_cast<Collider*>(contact->GetFixtureA()->GetUserData().pointer);
            b2WorldManifold info;
            contact->GetWorldManifold(&info);
            position = Vector2::from(info.points[0]);
            if (isA && info.normal.Length() != 0.F)
                normal = Vector2::from(info.normal).norm();
            else if (!isA && info.normal.Length() != 0.F)
                normal = -Vector2::from(info.normal).norm();
        }
    } // namespace phys
} // namespace stay

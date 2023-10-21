#include "collision.hpp"
#include "collider.hpp"

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
            position = utils::convertVec2<Vector2>(info.points[0]);
            if (isA)
                normal = utils::convertVec2<Vector2>(info.normal);
            else
                normal = -1.F * utils::convertVec2<Vector2>(info.normal);
        }
    } // namespace phys
} // namespace stay

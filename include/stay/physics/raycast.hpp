#pragma once

#include <box2d/b2_world.h>

#include "stay/type/vector.hpp"

namespace stay
{
    namespace phys
    {
        class Collider;
        class RaycastResult;
        class RaycastListener : public b2RayCastCallback
        {
            public:
                RaycastListener(RaycastResult& result);
                float ReportFixture(
                    b2Fixture* fixture, 
                    const b2Vec2& point, 
                    const b2Vec2& normal, 
                    float fraction) override;
            private:
                RaycastResult& mResult;
        };

        struct RaycastResult
        {
            struct Hit
            {
                float fraction;
                Vector2 point;
                Vector2 normal;
                std::reference_wrapper<Collider> collider;
                Hit(float fraction, Vector2 point, Vector2 normal, Collider& collider);

                Hit& operator = (const Hit&) = default;
            };

            const std::vector<Hit>& data() const;

            private:
                friend class RaycastListener;
                friend bool raycast(const Vector2&, const Vector2&, float, RaycastResult&);
                std::vector<Hit> mHits;
        };

        // @return `true` if the ray hits at least one collider
        // @param `result` contains intersections sorted by distance from `origin`
        bool raycast(const Vector2& origin, const Vector2& direction, float length, RaycastResult& result);
    } // namespace phys
} // namespace stay
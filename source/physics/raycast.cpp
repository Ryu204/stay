#include "stay/physics/raycast.hpp"

#include "stay/physics/collider.hpp"
#include "stay/physics/world.hpp"
#include <algorithm>

namespace stay
{
    namespace phys
    {
        RaycastListener::RaycastListener(RaycastResult& result)
            : mResult{result}
        {
            mResult.mHits.clear();
        }

        float RaycastListener::ReportFixture(
            b2Fixture* fixture, 
            const b2Vec2& point, 
            const b2Vec2& normal, 
            float fraction)
        {
            auto& collider = *reinterpret_cast<Collider*>(fixture->GetUserData().pointer);
            mResult.mHits.emplace_back(fraction, Vector2::from(point), Vector2::from(normal), collider);
            return 1.F;
        }

        RaycastResult::Hit::Hit(float fraction, Vector2 point, Vector2 normal, Collider& collider)
            : fraction{fraction}
            , point{std::move(point)}
            , normal{std::move(normal)}
            , collider{collider}
        {}

        const std::vector<RaycastResult::Hit>& RaycastResult::data() const
        {
            return mHits;
        }

        bool raycast(const Vector2& origin, const Vector2& direction, float length, RaycastResult& result)
        {
            auto& world = World::get();
            RaycastListener listener{result};
            const auto end = Vector2{origin + direction * length};
            world.RayCast(&listener, origin.toVec2<b2Vec2>(), end.toVec2<b2Vec2>());
            static const auto compare = [](const auto& a, const auto& b) {
                return a.fraction < b.fraction;
            };
            std::sort(result.mHits.begin(), result.mHits.end(), compare);
            return !result.data().empty();
        }
    } // namespace phys
} // namespace stay
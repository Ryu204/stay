#include "stay/physics/raycast.hpp"

#include "stay/utility/convert.hpp"
#include "stay/physics/world.hpp"
#include "stay/utility/math.hpp"

namespace stay 
{
    namespace phys
    {
        namespace detail
        {
            struct RaycastListener : b2RayCastCallback
            {
                enum class Type
                {
                    ALL, NEAREST,
                };
                RaycastListener(Type type, std::vector<Raycast::Info>& buffer, float length)
                    : mType{type}
                    , mBuffer{buffer} 
                    , mLength{length}
                {
                    mBuffer.clear();
                }

                float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                    const b2Vec2& /* normal */, float fraction) override
                {
                    auto* collider = reinterpret_cast<Collider*>(fixture->GetUserData().pointer);
                    mBuffer.emplace_back(*collider, Vector2::from(point), fraction * mLength);
                    switch (mType) 
                    {
                        case Type::ALL:
                            return 1.F;
                        case Type::NEAREST:
                            return fraction;
                        default:
                            return 1.F;
                    }
                }

                private:
                    Type mType{};
                    std::vector<Raycast::Info>& mBuffer;
                    float mLength{};
            };
        } // namespace detail

        Raycast::Info::Info(Collider& collider, Vector2 point, float distance)
            : collider{collider}
            , point{std::move(point)}
            , distance{distance}
        {}

        bool Raycast::Info::operator < (const Info& other) const
        {
            return distance < other.distance;
        }

        std::optional<Raycast::Info> Raycast::nearest(const Vector2& begin, const Vector2& direction, float length)
        {
            std::vector<Raycast::Info> res{};
            detail::RaycastListener listener{
                detail::RaycastListener::Type::NEAREST, res, length
            };
            const auto endPoint = utils::convertVec2<b2Vec2>(begin + length * direction.norm());
            getWorld().RayCast(&listener, begin.toVec2<b2Vec2>(), endPoint);
            std::sort(res.begin(), res.end());
            if (res.empty())
                return std::optional<Raycast::Info>();
            return std::make_optional<Raycast::Info>(res.front());
        }

        std::optional<Raycast::Info> Raycast::nearest(const Vector2& begin, const Vector2& end)
        {
            return nearest(begin, end - begin, utils::lengthVec2(end - begin));
        }

        std::vector<Raycast::Info> Raycast::all(const Vector2& begin, const Vector2& direction, float length)
        {
            std::vector<Raycast::Info> res{};
            detail::RaycastListener listener{
                detail::RaycastListener::Type::ALL, res, length
            };
            const auto endPoint = utils::convertVec2<b2Vec2>(begin + length * direction.norm());
            getWorld().RayCast(&listener, begin.toVec2<b2Vec2>(), endPoint);
            std::sort(res.begin(), res.end());
            return std::move(res);
        }
        std::vector<Raycast::Info> Raycast::all(const Vector2& begin, const Vector2& end)
        {
            return all(begin, end - begin, utils::lengthVec2(end - begin));
        }

        b2World& Raycast::getWorld()
        {
            return phys::World::get();
        }
    } // namespace phys
} // namespace stay

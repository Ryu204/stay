#pragma once

#include <functional>
#include <optional>

#include <box2d/box2d.h>

#include "stay/type/vector.hpp"
#include "collider.hpp"

namespace stay 
{
    namespace phys
    {
        class Raycast
        {
            public:
                // Single result of raycast operation
                struct Info
                {
                    Info(Collider& collider, Vector2 point, float distance);
                    std::reference_wrapper<Collider> collider;
                    Vector2 point;
                    float distance;
                    bool operator < (const Info& other) const;
                };
                static std::optional<Info> nearest(const Vector2& begin, const Vector2& direction, float length);
                static std::optional<Info> nearest(const Vector2& begin, const Vector2& end);
                static std::vector<Info> all(const Vector2& begin, const Vector2& direction, float length);
                static std::vector<Info> all(const Vector2& begin, const Vector2& end);
            private:
                static b2World& getWorld();
        };
    } // namespace phys
} // namespace stay

#pragma once

#include <variant>

#include <box2d/box2d.h>

#include "stay/type/vector.hpp"
#include "stay/type/serializable.hpp"

namespace stay
{
    namespace phys
    {
        struct Box : public Serializable
        {
            Box(Vector2 pos = Vector2(), Vector2 size = Vector2(1.F, 1.F), float angle = 0.F)
                : position(std::move(pos))
                , size(std::move(size))
                , angle(angle)
            {}
            Vector2 position;
            Vector2 size;
            float angle; // in game coords
            SERIALIZE(position, size, angle)
        };
        struct Circle : public Serializable
        {
            Circle(Vector2 position = Vector2(), float radius = 1.F)
                : position(std::move(position))
                , radius(radius)
            {}
            Vector2 position;
            float radius;
            SERIALIZE(position, radius)
        };
        struct Chain : public Serializable
        {
            Chain() = default;
            Chain(const std::vector<Vector2>& points);
            Chain(std::vector<Vector2>&& points);
            DESERIALIZE_POSTPROCESSING(mPoints);
            void postDeserialization();
            const std::vector<b2Vec2>& data() const;
            private:
                mutable bool mCached{false};
                std::vector<Vector2> mPoints{};
                mutable std::vector<b2Vec2> mData{};
        };
        struct ColliderInfo : public std::variant<Box, Circle, Chain>, public Serializable
        {
            using std::variant<Box, Circle, Chain>::variant;
            Serializable::Data serialize() const override;
            bool deserialization(const Serializable::Data& value) override;
            Uptr<b2Shape> createShape() const;
        };
    } // namespace phys
} // namespace stay
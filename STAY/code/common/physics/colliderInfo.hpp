#pragma once

#include <variant>

#include <box2d/box2d.h>

#include "../type/vector.hpp"
#include "../type/serializable.hpp"

namespace stay
{
    namespace phys
    {
        struct Box : public Serializable
        {
            Box(const Vector2& pos = Vector2(), const Vector2& size = Vector2(1.F, 1.F), float angle = 0.F)
            {
                position = pos;
                this->size = size;
                this->angle = angle;
            }
            Vector2 position;
            Vector2 size;
            float angle; // in game coords
            SERIALIZE(position, size, angle)
        };
        struct Circle : public Serializable
        {
            Circle(Vector2 position = Vector2(), float radius = 1.F)
            {
                this->position = position;
                this->radius = radius;
            }
            Vector2 position;
            float radius;
            SERIALIZE(position, radius)
        };
        struct Chain : public Serializable
        {
            Chain() = default;
            Chain(std::vector<Vector2>& points);
            SERIALIZE_POSTPROCESSING(mPoints);
            void postSerialization();
            const std::vector<b2Vec2>& data() const;
            private:
                mutable bool mCached{false};
                std::vector<Vector2> mPoints{};
                mutable std::vector<b2Vec2> mData{};
        };
        struct ColliderInfo : public std::variant<Box, Circle, Chain>, public Serializable
        {
            using std::variant<Box, Circle, Chain>::variant;
            Json::Value toJSONObject() const override;
            bool fetch(const Json::Value& value) override;
            Uptr<b2Shape> createShape() const;
        };
    } // namespace phys
} // namespace stay
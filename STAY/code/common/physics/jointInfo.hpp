#pragma once

#include <variant>

#include <box2d/box2d.h>

#include "type/serializable.hpp"
#include "type/vector.hpp"
#include "ecs/type.hpp"

namespace stay
{
    namespace phys
    {
        class RigidBody;
        struct Prismatic : Serializable
        {
            Prismatic(Vector2 anchorA = Vector2{0.F, 0.F}, Vector2 anchorB = Vector2{0.F, 3.F}, Vector2 axis = vectorDown);
            Vector2 anchorA;
            Vector2 anchorB;
            Vector2 axis;
            SERIALIZE(anchorA, anchorB, axis);
        };
        struct Revolute : Serializable
        {
            Revolute(Vector2 anchor);
            Vector2 anchor;
            SERIALIZE(anchor);
        };

        using JointData = std::variant<Prismatic, Revolute>;

        struct JointInfo : public Serializable
        {
            JointInfo(ecs::Entity other = ecs::Entity{0}, bool shouldCollide = false, JointData data = JointData{});
            Serializable::Data toJSONObject() const override;
            bool fetch(const Serializable::Data& value) override;
            Uptr<b2JointDef> createDef(RigidBody& a, RigidBody& b) const;

            struct Connect : Serializable 
            {
                Connect(bool shouldCollide, ecs::Entity other) 
                    : shouldCollide(shouldCollide)
                    , other(other)
                {}
                bool shouldCollide{false};
                ecs::Entity other{0};
                SERIALIZE(shouldCollide, other);
            } connectStatus;
            JointData data;
        };
    } // namespace phys
} // namespace stay
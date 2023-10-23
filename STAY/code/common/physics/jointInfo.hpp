#pragma once

#include <variant>

#include <box2d/b2_joint.h>

#include "../type/serializable.hpp"
#include "../type/vector.hpp"

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

        namespace detail 
        {
            using JointInfoBase = std::variant<Prismatic, Revolute>;
        } // namespace detail

        struct JointInfo : public detail::JointInfoBase, public Serializable
        {
            using detail::JointInfoBase::variant;
            Json::Value toJSONObject() const override;
            bool fetch(const Json::Value& value) override;
            Uptr<b2JointDef> createDef(RigidBody& A, RigidBody& B) const;
        };
    } // namespace phys
} // namespace stay
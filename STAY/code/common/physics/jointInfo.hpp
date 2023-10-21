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
            Prismatic(Vector2 anchor = Vector2{0.F, 0.F}, Vector2 axis = vectorDown);
            Vector2 anchor;
            Vector2 axis;
            SERIALIZE(anchor, axis);
        };
        struct Revolute : Serializable
        {

        };

        namespace detail 
        {
            using JointInfoBase = std::variant<Prismatic>;
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
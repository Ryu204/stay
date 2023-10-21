#pragma once

#include <box2d/b2_joint.h>
#include <box2d/b2_prismatic_joint.h>
#include <json/value.h>

#include "../ecs/component.hpp"
#include "../ecs/type.hpp"
#include "jointInfo.hpp"

namespace stay
{
    namespace phys
    {
        class RigidBody;

        enum class JointType
        {
            PRISMATIC = e_prismaticJoint,
            REVOLUTE = e_revoluteJoint,
            UNKNOWN = e_unknownJoint,
        };

        class Joint : public ecs::Component
        {
            public:
                void start(ecs::Entity other, const JointInfo& info, bool collide = true);
                RigidBody& body();
                RigidBody& other();
            private:
                b2Joint* mJoint{nullptr};
                RigidBody* mBody;
                RigidBody* mOther;
                Json::Value toJSONObject() const override {return Json::Value(); };
                bool fetch(const Json::Value& data) override {return true; };
        };
    } // namespace phys
} // namespace stay
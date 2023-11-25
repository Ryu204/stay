#pragma once

#include <box2d/b2_joint.h>
#include <box2d/b2_prismatic_joint.h>
#include <cassert>
#include <cstddef>
#include <json/value.h>
#include <vector>

#include "../ecs/component.hpp"
#include "../ecs/type.hpp"
#include "jointInfo.hpp"

namespace stay
{
    namespace phys
    {
        class RigidBody;

        class Joint : public ecs::Component
        {
            public:
                Joint() = default;
                ~Joint() override;
                void start(ecs::Entity other, const JointInfo& info, bool collide = true);
                RigidBody& body();
                RigidBody& other();
                template<typename T>
                T* getNativeHandle()
                {
                    check();
                    auto* res = dynamic_cast<T*>(mJoint);
                    assert(res != nullptr && "Get wrong internal joint type");
                    return res;
                }

                Json::Value toJSONObject() const override {return Json::Value(); };
                bool fetch(const Json::Value& data) override {return true; };
            private:
                void check() const;

                b2Joint* mJoint{nullptr};
                RigidBody* mBody{nullptr};
                RigidBody* mOther{nullptr};

                friend class DestructRegister;
        };
    } // namespace phys
} // namespace stay
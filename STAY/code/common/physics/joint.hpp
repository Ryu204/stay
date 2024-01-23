#pragma once

#include <optional>

#include "ecs/component.hpp"
#include "ecs/type.hpp"
#include "jointInfo.hpp"

namespace stay
{
    namespace phys
    {
        class RigidBody;

        class Joint : public ecs::Component<Joint>
        {
            public:
                Joint() = default;
                ~Joint() override;
                void start(const JointInfo& info);
                // Use joint information serialized from save file
                void start();
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
                COMPONENT_NAME(joint);
                SERIALIZE_POSTPROCESSING(mInfoCache);
                void postSerialization();
            private:
                void check() const;

                b2Joint* mJoint{nullptr};
                RigidBody* mBody{nullptr};
                RigidBody* mOther{nullptr};
                JointInfo mInfoCache{};
                bool mHasJointInfo{false};

                friend class DestructRegister;
        };
    } // namespace phys
} // namespace stay
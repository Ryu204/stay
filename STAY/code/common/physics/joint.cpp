#include "joint.hpp"
#include "jointInfo.hpp"
#include "rigidBody.hpp"
#include "world.hpp"

#include <cassert>

namespace stay
{
    namespace phys
    {
        Joint::~Joint()
        {
            if (mJoint != nullptr)
                World::get().DestroyJoint(mJoint);
        }
        void Joint::start(const JointInfo& info)
        {
            mBody = &getNode()->getComponent<RigidBody>();
            mOther = &Node::getNode(info.connectStatus.other)->getComponent<RigidBody>();
            auto def = info.createDef(*mBody, *mOther);
            def->collideConnected = info.connectStatus.shouldCollide;
            def->userData.pointer = reinterpret_cast<uintptr_t>(this);

            mJoint = World::get().CreateJoint(def.get());
            if (!mHasJointInfo) {
                mInfoCache = info;
                mHasJointInfo = true;
            }
        }

        void Joint::postSerialization() {
            mHasJointInfo = true;
        }

        void Joint::start() {
            assert(mHasJointInfo && "this joint did not receive data (via deserialization or direct start)");
            start(mInfoCache);
        }

        RigidBody& Joint::body()
        {
            check();
            return *mBody; 
        }

        RigidBody& Joint::other()
        {
            check();
            return *mOther;
        }

        void Joint::check() const
        {
            if (mJoint == nullptr)  
                throw std::runtime_error("access a joint with null data (probably has not started)");
        }
    } // namespace phys
} // namespace stay
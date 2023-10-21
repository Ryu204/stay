#include "joint.hpp"
#include "jointInfo.hpp"
#include "../world/node.hpp"
#include "rigidBody.hpp"
#include "world.hpp"
#include <cstddef>

namespace stay
{
    namespace phys
    {
        Joint::~Joint()
        {
            if (World::avail() && mJoint != nullptr)
                World::get().DestroyJoint(mJoint);
            if (mOther != nullptr)
                mOther->OnRemoval.removeListener(mOtherEventID);
            if (mBody != nullptr)
                mBody->OnRemoval.removeListener(mBodyEventID);
        }
        void Joint::start(ecs::Entity other, const JointInfo& info, bool collide)
        {
            mBody = &getNode()->getComponent<RigidBody>();
            mOther = &Node::getNode(other)->getComponent<RigidBody>();
            auto def = info.createDef(*mBody, *mOther);
            def->collideConnected = collide;
            def->userData.pointer = reinterpret_cast<uintptr_t>(this);

            mJoint = World::get().CreateJoint(def.get());

            // Auto erase if either body gets destructed
            mBodyEventID = mBody->OnRemoval.addEventListener([&]() {
                delete this;
            });
            mOtherEventID = mOther->OnRemoval.addEventListener([&]() {
                delete this;
            });
        }

        RigidBody& Joint::body()
        {
            return *mBody; 
        }

        RigidBody& Joint::other()
        {
            return *mOther;
        }
    } // namespace phys
} // namespace stay
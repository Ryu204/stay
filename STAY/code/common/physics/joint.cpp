#include "joint.hpp"
#include "jointInfo.hpp"
#include "../world/node.hpp"
#include "rigidBody.hpp"
#include "world.hpp"
#include <cstddef>
#include <stdexcept>

namespace stay
{
    namespace phys
    {
        Joint::~Joint()
        {
            if (mJoint != nullptr)
                World::get().DestroyJoint(mJoint);
        }
        void Joint::start(ecs::Entity other, const JointInfo& info, bool collide)
        {
            mBody = &getNode()->getComponent<RigidBody>();
            mOther = &Node::getNode(other)->getComponent<RigidBody>();
            auto def = info.createDef(*mBody, *mOther);
            def->collideConnected = collide;
            def->userData.pointer = reinterpret_cast<uintptr_t>(this);

            mJoint = World::get().CreateJoint(def.get());
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
                throw std::runtime_error("acess a joint with null data (probably has not started)");
        }
    } // namespace phys
} // namespace stay
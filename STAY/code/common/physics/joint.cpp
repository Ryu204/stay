#include "joint.hpp"
#include "jointInfo.hpp"
#include "../world/node.hpp"
#include "rigidBody.hpp"
#include "world.hpp"

namespace stay
{
    namespace phys
    {
        void Joint::start(ecs::Entity other, const JointInfo& info, bool collide)
        {
            mBody = &getNode()->getComponent<RigidBody>();
            mOther = &Node::getNode(other)->getComponent<RigidBody>();
            auto def = info.createDef(*mBody, *mOther);
            def->collideConnected = collide;
            def->userData.pointer = reinterpret_cast<uintptr_t>(this);

            World::get().CreateJoint(def.get());

            // Auto erase if either body gets destructed
            mBody->OnRemoval.addEventListener([this]() {
                getNode()->removeComponents<Joint>();
            });
            mOther->OnRemoval.addEventListener([this]() {
                getNode()->removeComponents<Joint>();
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
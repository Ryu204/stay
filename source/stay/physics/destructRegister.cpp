#include "stay/physics/destructRegister.hpp"
#include "stay/physics/collider.hpp"
#include "stay/physics/joint.hpp"

namespace stay 
{
    namespace phys
    {
        void DestructRegister::SayGoodbye(b2Fixture* fixture)
        {
            auto& collider = *reinterpret_cast<Collider*>(fixture->GetUserData().pointer);
            collider.mFixture = nullptr;
            // bye bye collider
            collider.getNode()->removeComponents<Collider>();
        }

        void DestructRegister::SayGoodbye(b2Joint* joint)
        {
            auto& jnt = *reinterpret_cast<Joint*>(joint->GetUserData().pointer);
            jnt.mJoint = nullptr;
            // bye bye joint
            jnt.getNode()->removeComponents<Joint>();
        }
    }
}
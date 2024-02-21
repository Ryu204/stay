#pragma once

#include <box2d/b2_world_callbacks.h>

namespace stay
{
    namespace phys
    {
        class DestructRegister : public b2DestructionListener
        {
            public:
                void SayGoodbye(b2Fixture* fixture) override;
                void SayGoodbye(b2Joint* joint) override;
        };
    } // namespace phys
} // namespace stay
#pragma once

#include "../../common/ecs/component.hpp"

namespace stay
{
    struct PlayerDebug : public ecs::Component
    {
            PlayerDebug() = default;
            SERIALIZE(foo);
        private:
            int foo;
    };
} // namespace stay
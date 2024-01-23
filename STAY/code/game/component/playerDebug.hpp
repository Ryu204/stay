#pragma once

#include "../../common/ecs/component.hpp"

namespace stay
{
    struct PlayerDebug : public ecs::Component<PlayerDebug>
    {
            PlayerDebug() = default;
            COMPONENT(playerDebug, foo);
        private:
            int foo;
    };
} // namespace stay
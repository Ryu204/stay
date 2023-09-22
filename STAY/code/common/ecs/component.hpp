#pragma once

#include <SFML/System/NonCopyable.hpp>

#include "system.hpp"
#include "../utility/assignable.hpp"

namespace stay
{
    class Node;
    namespace ecs
    {
        class Manager;
        // Every component type must inherit this struct
        struct Component : utils::Assignable<Entity>, sf::NonCopyable
        {
                Node* getNode() const;
                virtual ~Component() = default;
            private:
                friend class Manager;
                friend class Node;
        };
    } // namespace ecs
} // namespace stay

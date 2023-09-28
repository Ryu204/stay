#pragma once

#include <SFML/System/NonCopyable.hpp>

#include "system.hpp"
#include "../utility/assignable.hpp"
#include "../type/serializable.hpp"

namespace stay
{
    class Node;
    namespace ecs
    {
        class Manager;
        // Every component type must inherit this struct
        struct Component : public Serializable, utils::Assignable<Entity>, sf::NonCopyable
        {
                using utils::Assignable<Entity>::get;
                Node* getNode() const;
                virtual ~Component() = default;
        };
    } // namespace ecs
} // namespace stay

#include "component.hpp"
#include "../world/node.hpp"

namespace stay
{
    namespace ecs
    {
        Node* Component::getNode() const
        {
            return Node::getNode(get());
        };
    } // namespace ecs
} // namespace stay

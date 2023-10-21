#include "component.hpp"
#include "../world/node.hpp"

namespace stay
{
    namespace ecs
    {
        Component::~Component()
        {
            OnRemoval.invoke();
        }
        
        Node* Component::getNode() const
        {
            return Node::getNode(get());
        };
    } // namespace ecs
} // namespace stay

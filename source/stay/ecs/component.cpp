#include "stay/ecs/component.hpp"
#include "stay/world/node.hpp"

namespace stay
{
    namespace ecs
    {
        Node* Component::getNode() const
        {
            return Node::getNode(get());
        };
        Component::~Component()
        {
            OnRemoval.invoke();
        }
    } // namespace ecs
} // namespace stay


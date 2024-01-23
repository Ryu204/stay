#pragma once

#include "componentFwd.hpp"
#include "componentLoader.hpp"
#include "world/nodeFwd.hpp"

namespace stay
{
    class Node;
    namespace ecs
    {
        template <typename T>
        Node* Component<T>::getNode() const
        {
            return Node::getNode(get());
        };
        template <typename T>
        Component<T>::~Component()
        {
            OnRemoval.invoke();
        }
        template <typename T>
        // NOLINTNEXTLINE(modernize-use-equals-default)
        Component<T>::SelfRegister::SelfRegister() {
            componentsLoader().registerComponent<T>(T::componentName());
        }
    } // namespace ecs
} // namespace stay
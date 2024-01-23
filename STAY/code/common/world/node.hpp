#pragma once

#include "nodeFwd.hpp"

namespace stay
{
    template <typename Func, typename... Args>
    void Node::visit(const Func& func, Args&&... args)
    {
        func(this, std::forward<Args>(args)...);
        for (auto& child : mChildren)
        {
            getNode(child.first)->visit(func, std::forward<Args>(args)...);
        }
    }
    
    template <typename Func, typename FuncReturn, typename... Args>
    void Node::visitChained(const Func& func, const FuncReturn& initial, Args&&... args)
    {
        const FuncReturn result = func(this, initial, std::forward<Args>(args)...);
        for (auto& child : mChildren)
        {
            getNode(child.first)->visitChained(func, result, std::forward<Args>(args)...);
        }
    }

    template <typename Type, typename... Args, std::enable_if_t<std::is_base_of_v<ecs::Component<Type>, Type>, bool>>
    Type& Node::addComponent(Args&&... args)
    {
        auto& res = globalInfo().registry->emplace<Type>(mEntity, std::forward<Args>(args)...);
        res.assign(mEntity);
        return res;
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component<Type>, Type>, bool>>
    void Node::removeComponents()
    {
        globalInfo().registry->remove<Type>(mEntity);
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component<Type>, Type>, bool>>
    bool Node::hasComponent() const
    {
        return globalInfo().registry->try_get<Type>(mEntity) != nullptr;
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component<Type>, Type>, bool>>
    Type& Node::getComponent()
    {
        assert(hasComponent<Type>() && "non-existing component");
        return globalInfo().registry->get<Type>(mEntity);
    }
} // namespace stay

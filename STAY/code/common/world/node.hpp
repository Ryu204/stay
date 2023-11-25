#pragma once

#include <vector>
#include <unordered_map>

#include "SFML/System/NonCopyable.hpp"

#include "transform.hpp"
#include "../utility/typedef.hpp"
#include "../utility/assignable.hpp"
#include "../ecs/system.hpp"
#include "../ecs/component.hpp"

namespace stay
{
    class Node : sf::NonCopyable
    {
        public:
            static void init(const SPtr<ecs::Registry>& registry);
            static void shutdown();
            static Node* getNode(ecs::Entity entity);

            Node();
            Node(ecs::Entity id);
            virtual ~Node();
            bool stray() const;
            const Node* parent() const;
            Node* parent();
            void setParent(Node* newParent);
            void setParent(ecs::Entity parent);
            void destroy(Node* child);
            void destroy(ecs::Entity child);
            void destroyChildren();
            bool childOf(const Node* node) const;
            bool parentOf(const Node* node) const;
            Node* createChild();
            Node* createChild(ecs::Entity id);
            std::vector<Node*> getChildren();

            const Transform& localTransform() const;
            Transform& localTransform();
            Transform globalTransform() const;
            void setLocalTransform(const Transform& transform);
            void setGlobalTransform(Transform& transform);

            template <typename Func, typename... Args>
            void visit(const Func& func, Args&&... args);
            template <typename Func, typename FuncReturn, typename... Args>
            void visitChained(const Func& func, const FuncReturn& initial, Args&&... args);
            
            ecs::Entity entity() const;
            template <typename Type, typename... Args, whereIs(Type, ecs::Component)>
            Type& addComponent(Args&&... args);
            template <typename Type, whereIs(Type, ecs::Component)>
            void removeComponents();
            template <typename Type, whereIs(Type, ecs::Component)>
            bool hasComponent() const;
            template <typename Type, whereIs(Type, ecs::Component)>
            Type& getComponent();
        private:
            struct Global
            {
                SPtr<ecs::Registry> registry;
                std::unordered_map<ecs::Entity, Node*> nodeOf{};
                ecs::Entity root;
            };
            static Global& globalInfo();
            
            ecs::Entity mParent;
            const ecs::Entity mEntity;
            std::unordered_map<ecs::Entity, Uptr<Node>> mChildren;
            Transform mLocalTransform;
    };

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

    template <typename Type, typename... Args, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    Type& Node::addComponent(Args&&... args)
    {
        auto& res = globalInfo().registry->emplace<Type>(mEntity, std::forward<Args>(args)...);
        res.assign(mEntity);
        return res;
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    void Node::removeComponents()
    {
        globalInfo().registry->remove<Type>(mEntity);
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    bool Node::hasComponent() const
    {
        return globalInfo().registry->try_get<Type>(mEntity) != nullptr;
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    Type& Node::getComponent()
    {
        assert(hasComponent<Type>() && "non-existing component");
        return globalInfo().registry->get<Type>(mEntity);
    }
} // namespace stay

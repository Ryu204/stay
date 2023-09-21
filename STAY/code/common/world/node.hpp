#pragma once

#include <vector>
#include <unordered_map>

#include "transform.hpp"
#include "../utility/typedef.hpp"
#include "../utility/assignable.hpp"
#include "../ecs/manager.hpp"

#include <iostream>

namespace stay
{
    class Node : public utils::Assignable<ecs::Manager*>
    {
        public:
            // The root node of the entire scene
            static Node& root();
            // Create an empty children of root node
            static Node* create();
            // Get the Node associating with `identifier`
            static Node* getNode(ecs::Entity identifier);
            // Comletely destroy a node and its children
            static void destroy(Node* node);

            virtual ~Node();
            void setParent(Node* newParent);
            void destroyChild(Node* child);
            void clearChildren();
            bool isChildOf(const Node* node) const;
            bool isParentOf(const Node* node) const;
            Node* createEmptyChild();

            Transform& getLocalTransform();
            const Transform& getLocalTransform() const;
            Transform getGlobalTransform() const;
            void setLocalTransform(const Transform& transform);
            void setGlobalTransform(Transform& transform);

            // Apply a function to each node of the subtree
            template <typename Func, typename... Args>
            void visit(const Func& func, Args&&... args);
            // Apply a function to each node of the subtree
            // The return of `func` in a node will be forwarded when applying `func` to its children
            template <typename Func, typename FuncReturn, typename... Args>
            void visitChained(const Func& func, FuncReturn initial, Args&&... args);
            
            // ecs-related methods
            ecs::Entity getEntity() const;
            // @brief Construct a component from its constructor args
            template <typename Type, typename... Args>
            auto addComponents(Args&&... args) ->decltype(auto);
            template <typename Type, typename... Other>
            void removeComponents();
            template <typename Type>
            bool hasComponent();
            template <typename Type>
            Type& getComponent();
        private:
            // Assignable related functions
            void postAssignment() override;
            static std::unordered_map<ecs::Entity, Node*>& globalMap();
            friend std::unique_ptr<Node> std::make_unique<Node>(); // NOLINT
            friend std::unique_ptr<Node>;
            Node();
            
            Node* mParent;
            ecs::Entity mEntity;
            std::unordered_map<Node*, Uptr<Node>> mChildren;
            Transform mLocalTransform;
    };

    template <typename Func, typename... Args>
    void Node::visit(const Func& func, Args&&... args)
    {
        func(this, std::forward<Args>(args)...);
        for (auto& child : mChildren)
        {
            child.first->visit(func, std::forward<Args>(args)...);
        }
    }
    template <typename Func, typename FuncReturn, typename... Args>
    void Node::visitChained(const Func& func, FuncReturn initial, Args&&... args)
    {
        FuncReturn result = func(this, initial, std::forward<Args>(args)...);
        for (auto& child : mChildren)
        {
            child.first->visitChained(func, result, std::forward<Args>(args)...);
        }
    }

    template <typename Type, typename... Args>
    auto Node::addComponents(Args&&... args) -> decltype(auto)
    {
        return get()->getRegistry().emplace<Type>(mEntity, std::forward<Args>(args)...);
    }

    template <typename Type, typename... Other>
    void Node::removeComponents()
    {
        get()->getRegistry().remove<Type, Other...>(mEntity);
    }

    template <typename Type>
    bool Node::hasComponent()
    {
        return get()->getRegistry().try_get<Type>(mEntity) != nullptr;
    }
    
    template <typename Type>
    Type& Node::getComponent()
    {
        assert(hasComponent<Type>() && "non-existing component");
        return get()->getRegistry().get<Type>(mEntity);
    }
} // namespace stay

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
            ecs::Entity getEntity() const;
        private:
            // Assignable related functions
            void postAssignment() override;
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
} // namespace stay

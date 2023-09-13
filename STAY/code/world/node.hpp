#pragma once

#include <vector>
#include <unordered_map>

#include "transform.hpp"
#include "../utility/typedef.hpp"

#include <iostream>

namespace stay
{
    class Node
    {
        public:
            // The root node of the entire scene
            static Node& root();
            // Create an empty children of root node
            static Node* create();
            // Comletely destroy a node and its children
            static void destroy(Node* node);

            virtual ~Node() = default;
            void setParent(Node* newParent);
            void destroyChild(Node* child);
            void clearChildren();
            bool isChildOf(const Node* node) const;
            bool isParentOf(const Node* node) const;
            Node* createEmptyChild();

            Transform& getLocalTransform();
            Transform getGlobalTransform() const;
            void setLocalTransform(const Transform& transform);
            void setGlobalTransform(const Transform& transform);
        private:
            friend std::unique_ptr<Node> std::make_unique<Node>(); // NOLINT
            friend std::unique_ptr<Node>;
            Node();

            Node* mParent;
            std::unordered_map<Node*, Uptr<Node>> mChildren;
            Transform mLocalTransform;
    };
} // namespace stay

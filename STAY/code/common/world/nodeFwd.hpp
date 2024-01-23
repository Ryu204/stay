#pragma once

#include "transform.hpp"
#include "ecs/componentFwd.hpp"

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
            template <typename Type, typename... Args, whereIs(Type, ecs::Component<Type>)>
            Type& addComponent(Args&&... args);
            template <typename Type, whereIs(Type, ecs::Component<Type>)>
            void removeComponents();
            template <typename Type, whereIs(Type, ecs::Component<Type>)>
            bool hasComponent() const;
            template <typename Type, whereIs(Type, ecs::Component<Type>)>
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
} // namespace stay

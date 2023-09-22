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
    class Node : utils::Assignable<ecs::Registry*>, sf::NonCopyable
    {
        public:
            // @brief The root node of the entire scene
            // @warning You should not do anything with it, except visiting (since it's only used for getting all current Node's refs)
            static Node& root();
            // @brief Use this function to associate a registry with node hierachy
            static void setGlobalRegistry(ecs::Registry* registry);
            // Create a children of root node using root's `ecs::Manager`
            static Node* create();
            // Get the Node associating with `identifier`
            static Node* getNode(ecs::Entity identifier);
            // Comletely destroy a node and its children
            // @warning Should not be used when a system is iterating
            static void destroy(Node* node);

            virtual ~Node();
            void setParent(Node* newParent);
            void destroyChild(Node* child);
            void clearChildren();
            bool isChildOf(const Node* node) const;
            bool isParentOf(const Node* node) const;
            // @note new child also functions with the parent's `ecs::Manager*`
            Node* createEmptyChild();

            Transform& getLocalTransform();
            const Transform& getLocalTransform() const;
            Transform getGlobalTransform() const;
            void setLocalTransform(const Transform& transform);
            void setGlobalTransform(Transform& transform);

            // Apply a function to each node of the subtree
            // @note `func` parameters are `(Node*, Args&&...)`
            template <typename Func, typename... Args>
            void visit(const Func& func, Args&&... args);
            // Apply a function to each node of the subtree
            // The return of `func` in a node will be forwarded when applying `func` to its children
            // @note `func` parameters are `(Node*, Args&&...)`
            template <typename Func, typename FuncReturn, typename... Args>
            void visitChained(const Func& func, const FuncReturn& initial, Args&&... args);
            
            ecs::Entity getEntity() const;
            // @brief Construct a component from its constructor args
            template <typename Type, typename... Args, whereIs(Type, ecs::Component)>
            Type& addComponents(Args&&... args);
            template <typename Type, whereIs(Type, ecs::Component)>
            void removeComponents();
            template <typename Type, whereIs(Type, ecs::Component)>
            bool hasComponent() const;
            template <typename Type, whereIs(Type, ecs::Component)>
            Type& getComponent();
        private:
            Node();
            static std::unordered_map<ecs::Entity, Node*>& globalMap();
            friend std::unique_ptr<Node> std::make_unique<Node>();
            friend std::unique_ptr<Node>;
            
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
    void Node::visitChained(const Func& func, const FuncReturn& initial, Args&&... args)
    {
        const FuncReturn result = func(this, initial, std::forward<Args>(args)...);
        for (auto& child : mChildren)
        {
            child.first->visitChained(func, result, std::forward<Args>(args)...);
        }
    }

    template <typename Type, typename... Args, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    Type& Node::addComponents(Args&&... args)
    {
        auto& res = get()->emplace<Type>(mEntity, std::forward<Args>(args)...);
        res.assign(mEntity);
        return res;
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    void Node::removeComponents()
    {
        get()->remove<Type>(mEntity);
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    bool Node::hasComponent() const
    {
        return get()->try_get<Type>(mEntity) != nullptr;
    }
    template <typename Type, std::enable_if_t<std::is_base_of_v<ecs::Component, Type>, bool>>
    Type& Node::getComponent()
    {
        assert(hasComponent<Type>() && "non-existing component");
        return get()->get<Type>(mEntity);
    }
} // namespace stay

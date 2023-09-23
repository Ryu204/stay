#include <string>

#include "node.hpp"
namespace stay
{
    Node::Node()
        : mParent(nullptr)
    { }

    Node::~Node()
    {
        // If root is destroyed, i.e program ends, everything is destroyed already so no more cleanup
        if (this == &root())
            return;
        // Order of removal: registry destroys entity, components -> destroy node and its global registration
        get()->destroy(mEntity);
        globalMap().erase(mEntity);
    }

    Node& Node::root()
    {
        static Node res;
        return res;
    };
    
    void Node::setGlobalRegistry(ecs::Registry* registry)
    {
        root().assign(registry);
    }

    void Node::postAssignment()
    {
        if (get() == nullptr)
        {
            return;
        }
        mEntity = get()->create();
        globalMap().emplace(mEntity, this);
    }

    Node* Node::create()
    {
        return root().createEmptyChild();
    }

    Node* Node::getNode(ecs::Entity identifier)
    {
        if (globalMap().find(identifier) == globalMap().end())
        {
            throw std::invalid_argument("Node: \"getNode\" called with non-existing identifier " + std::to_string(static_cast<int>(identifier)));
        }
        return globalMap().at(identifier);
    }

    std::unordered_map<ecs::Entity, Node*>& Node::globalMap()
    {
        static std::unordered_map<ecs::Entity, Node*> res;
        return res;
    }
    
    void Node::destroy(Node* node)
    {
        assert(node != &root() && "cannot destroy root");
        assert(node != nullptr && "cannot destroy null node");
        node->mParent->destroyChild(node);
    }
            

    void Node::setParent(Node* newParent)
    {
        assert(this != &root() && "cannot change root's parent");
        assert(newParent != nullptr && "cannot set null as parent\n");
        if (newParent->isParentOf(this))
            return;
        assert(newParent->isChildOf(this) == false && "a cycle exists in the scene");
        // move unique_ptr to new parent container
        newParent->mChildren.emplace(this, std::move(mParent->mChildren[this]));
        // detach from previous parent
        mParent->mChildren.erase(this);
        // set new parent
        mParent = newParent;
    }

    void Node::destroyChild(Node* child)
    {
        assert(mChildren.find(child) != mChildren.end() && "no child found to destroy");
        mChildren.erase(child);
    }

    void Node::clearChildren()
    {
        mChildren.clear();
    }

    bool Node::isChildOf(const Node* node) const
    {
        if (node == &root())
        {
            return true;
        }
        if (node == nullptr)
        {
            return false;
        }
        const auto* parent = this;
        // the tree's depth won't be too great so we can brute-force from the child upwards
        while (parent != nullptr)
        {
            if (parent == node)
            {
                return true;
            }
            parent = parent->mParent;
        }
        return false;
    }

    bool Node::isParentOf(const Node* node) const
    {
        return node->isChildOf(this);
    }

    Node* Node::createEmptyChild()
    {
        auto ptr = std::make_unique<Node>();
        auto* res = ptr.get();
        res->mParent = this;
        mChildren.emplace(res, std::move(ptr));
        // Once set up, the parent's registry pointer will be passed down to child as well
        res->assign(get());
        return res;
    }

    Transform& Node::getLocalTransform()
    {
        assert(this != &root() && "cannot modify root's transform");
        return mLocalTransform;
    }

    const Transform& Node::getLocalTransform() const
    {
        return mLocalTransform;
    }

    Transform Node::getGlobalTransform() const
    {
        auto res = mLocalTransform;
        auto* parent = mParent;
        while (parent != nullptr && parent != &root())
        {
            res.setMatrix(parent->mLocalTransform.getMatrix() * res.getMatrix());
            parent = parent->mParent;
        }
        return res;
    }

    void Node::setLocalTransform(const Transform& transform)
    {
        mLocalTransform = transform;
    }

    void Node::setGlobalTransform(Transform& transform)
    {
        mLocalTransform.setMatrix(mParent->getGlobalTransform().getInverseMatrix() * transform.getMatrix());
    }

    ecs::Entity Node::getEntity() const
    {
        return mEntity;
    }
} // namespace stay

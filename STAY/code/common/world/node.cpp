#include "node.hpp"

namespace stay
{
    Node::Node()
        : mParent(nullptr)
    {}
    Node::~Node()
    {
        if (assigned() && this != &root())
        {
            get()->getRegistry().destroy(mEntity);
            globalMap().erase(getEntity());
        }
    }
    Node& Node::root()
    {
        static Node res;
        return res;
    };
    
    Node* Node::create()
    {
        return root().createEmptyChild();
    }

    Node* Node::getNode(ecs::Entity identifier)
    {
        return globalMap()[identifier];
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

    void Node::postAssignment()
    {
        mEntity = get()->getRegistry().create();
        globalMap()[mEntity] = this;
    }

    ecs::Entity Node::getEntity() const
    {
        return mEntity;
    }
} // namespace stay

#include <string>

#include "node.hpp"

namespace stay
{
    void Node::init(const SPtr<ecs::Registry>& registry)
    {
        auto& info = globalInfo();
        info.nodeOf.clear();
        info.registry = registry;
        info.root = registry->create();
    }

    void Node::shutdown()
    {
        auto& info = globalInfo();
        info.nodeOf.clear();
        info.registry->destroy(info.root);
        info.registry.reset();
    }

    Node* Node::getNode(ecs::Entity entity)
    {
        assert(globalInfo().nodeOf.find(entity) != globalInfo().nodeOf.end());
        return globalInfo().nodeOf.at(entity);
    }

    Node::Global& Node::globalInfo()
    {
        static Global global;
        return global;
    }   

    Node::Node()
        : mParent(globalInfo().root)
        , mEntity(globalInfo().registry->create())
    { 
        globalInfo().nodeOf.emplace(mEntity, this);
    }

    Node::Node(ecs::Entity id)
        : mParent(globalInfo().root)
        , mEntity(globalInfo().registry->create(id))
    {
        assert(mEntity == id && "specified ID alreay exists");
        globalInfo().nodeOf.emplace(mEntity, this);
    }

    Node::~Node()
    {
        globalInfo().registry->destroy(mEntity);
        globalInfo().nodeOf.erase(mEntity);
    }

    bool Node::stray() const
    {
        return mParent == globalInfo().root;
    }

    const Node* Node::parent() const
    {
        assert(stray() == false && "top-level node has no parent");
        return getNode(mParent);
    }

    Node* Node::parent() 
    {
        return const_cast<Node*>(std::as_const(*this).parent());
    }

    void Node::setParent(Node* newParent)
    {
        assert(stray() == false && "cannot set parent of top-level node");
        assert(newParent != nullptr && "cannot set null as parent");
        assert(newParent->childOf(this) == false && "a cycle exists in the scene");

        bool alreadyParent = newParent->mEntity == mParent;
        if (alreadyParent)
            return;
        // move unique_ptr to new parent container
        newParent->mChildren.emplace(mEntity, std::move(parent()->mChildren.at(mEntity)));
        // detach from previous parent
        parent()->mChildren.erase(mEntity);
        // set new parent
        mParent = newParent->mEntity;
    }

    void Node::setParent(ecs::Entity parent)
    {
        assert(parent != globalInfo().root && "Cannot change stray-ness of a node");
        
        setParent(getNode(parent));
    }

    void Node::destroy(Node* child)
    {
        assert(child != nullptr);
        destroy(child->mEntity);
    }

    void Node::destroy(ecs::Entity child)
    {
        assert(mChildren.find(child) != mChildren.end() && "no child found to destroy");
        mChildren.erase(child);
    }

    void Node::destroyChildren()
    {
        mChildren.clear();
    }

    bool Node::childOf(const Node* node) const
    {
        if (node == nullptr)
        {
            return false;
        }
        auto parent = mEntity;
        const auto other = node->mEntity;
        while (parent != globalInfo().root)
        {
            if (parent == other)
            {
                return true;
            }
            parent = getNode(parent)->mParent;
        }
        return false;
    }

    bool Node::parentOf(const Node* node) const
    {
        return node->childOf(this);
    }

    Node* Node::createChild()
    {
        auto ptr = std::make_unique<Node>();
        auto* res = ptr.get();
        res->mParent = mEntity;
        mChildren.emplace(res->mEntity, std::move(ptr));
        return res;
    }

    Node* Node::createChild(ecs::Entity id)
    {
        auto ptr = std::make_unique<Node>(id);
        auto* res = ptr.get();
        res->mParent = mEntity;
        mChildren.emplace(res->mEntity, std::move(ptr));
        return res;
    }

    const Transform& Node::localTransform() const
    {
        return mLocalTransform;
    }

    Transform& Node::localTransform()
    {
        return mLocalTransform;
    }

    Transform Node::globalTransform() const
    {
        auto res = mLocalTransform.getMatrix();
        auto parent = mParent;
        while (parent != globalInfo().root)
        {
            res = getNode(parent)->mLocalTransform.getMatrix() * res;
            parent = getNode(parent)->mParent;
        }
        return Transform(res);
    }

    void Node::setLocalTransform(const Transform& transform)
    {
        mLocalTransform = transform;
    }

    void Node::setGlobalTransform(Transform& transform)
    {
        if (stray())
        {
            setLocalTransform(transform);
            return;
        }
        mLocalTransform.setMatrix(parent()->globalTransform().getInverseMatrix() * transform.getMatrix());
    }

    ecs::Entity Node::entity() const
    {
        return mEntity;
    }
} // namespace stay

#include "node.hpp"
#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "../../game/component/list.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene()
        : mPhysicsWorld({0.F, -10.F})
        , mPixelsPerMeter(100.F)
    {
        Node::root().assign(&mManager);
        initialize();
    }
    Scene::~Scene()
    {
        Node::root().clearChildren();
    }
    void Scene::update(float dt)
    {
        mManager.update(dt);
    }
    void Scene::start()
    {
        mManager.start();
    }
    void Scene::input(const sf::Event& event)
    {
        mManager.input(event);
    }
    void Scene::render(RTarget* target)
    {
        mCamera.setOn(target);
        mManager.render(target);
    }
    void Scene::initialize()
    {
        // mManager.registerSystem<sys::RawRenderSystem>();
        mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>()->initialize(&mPhysicsWorld);
        mManager.registerSystem<sys::PhysicsSystem>()->initialize(&mPhysicsWorld);
        
        auto* node = create();
        auto& body = node->addComponents<phys::RigidBody>(&mPhysicsWorld, Vector2(0, 0), 45, phys::BodyType::KINEMATIC);
        body.setAngularVelocity(100);
        node->addComponents<phys::Collider>(phys::Collider::Box{Vector2(0, 0), Vector2(1.F, 2.F)}, &body, nullptr);
        node->addComponents<comp::Render>(sf::Color::Black, sf::Vector2f{0.5F, 1.F});
        for (int i = 1; i <= 4; ++i)
        {
            const auto& trans = node->getLocalTransform().getMatrix();
            node = createChild(node);
            node->addComponents<comp::Render>(sf::Color::Red, sf::Vector2f{0.4F, 1.2F});
            node->getLocalTransform().move({1.3F, 1.3F, 0});
            node->getLocalTransform().rotate(10);
        }
    }

    Node* Scene::create()
    {
        auto* res = Node::create();
        res->assign(&mManager);
        return res;
    }

    Node* Scene::createChild(Node* node)
    {
        auto* res = create();
        res->setParent(node);
        return res;
    }
} // namespace stay

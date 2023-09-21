#include "node.hpp"
#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "../../game/component/list.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene()
        : mCamera(Vector2(), Vector2(500, 500))
        , mPhysicsWorld({0.F, -10.F})
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
        mCamera = Camera::defaultOfTarget(*target);
        target->setView(mCamera.getView());
        mManager.render(target);
    }
    void Scene::initialize()
    {
        // mManager.registerSystem<sys::RawRenderSystem>();
        // mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::RawPhysicsRenderSystem>()->initialize(&mPhysicsWorld);
        auto* node = create();

        auto& body = node->addComponents<phys::RigidBody>(&mPhysicsWorld, Vector2(0, 0), 45, phys::BodyType::DYNAMIC);
        body.setAngularVelocity(100);
        auto& ref = node->addComponents<phys::Collider>(phys::Collider::Box{Vector2(0, 0), Vector2(1.F, 2.F)}, &body, nullptr);
        // node->addComponents<comp::Render>(sf::Color::Black, sf::Vector2f{40, 60});
        //node->getLocalTransform().move({100, -100, 0});
        for (int i = 1; i <= 4; ++i)
        {
            node = createChild(node);
            // node->addComponents<comp::Render>(sf::Color::Red, sf::Vector2f{70, 50});
            //node->getLocalTransform().move({35, 35, 0});
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

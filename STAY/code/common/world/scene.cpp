#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "../../game/component/list.hpp"
#include "camera.hpp"
#include "scene.hpp"
/*debug*/ #include <iostream>
/*debug*/ #include <fstream>

namespace stay
{
    Scene::Scene()
        : mPhysicsWorld({0.F, -10.F})
        , mPixelsPerMeter(100.F)
    {
        Node::init(&mManager.getRegistry());
        mSceneRoot = std::make_unique<Node>();
        initialize();
    }
    Scene::~Scene()
    {
        mSceneRoot.reset();
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
        mManager.render(target, mSceneRoot.get());
    }
    void Scene::initialize()
    {
        // mManager.registerSystem<sys::RawRenderSystem>();
        mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>()->initialize(&mPhysicsWorld);
        mManager.registerSystem<sys::PhysicsSystem>()->initialize(&mPhysicsWorld);
        //======================================debug==============================
         /*debug:test component loading*/
        Json::Value obj;
        std::ifstream reader("asset/test.json");
        reader >> obj;
        ComponentsLoader compLoader(&mManager);
        compLoader.registerComponent<comp::Render>("render");
        compLoader.registerComponent<phys::RigidBody>("rigidbody");
        compLoader.registerComponent<phys::Collider>("collider");

        auto* node = mSceneRoot->createChild();
        compLoader.loadAllComponents(node->entity(), obj["componentsData"]);
        std::ofstream("asset/out.json") << compLoader.saveAllComponents(node->entity());

        node = mSceneRoot->createChild();
        auto& body1 = node->addComponent<phys::RigidBody>(Vector2(0, -6), 0, phys::BodyType::STATIC);
        auto& col1 = node->addComponent<phys::Collider>(phys::Collider::Box{Vector2(0, 0), Vector2(4.F, 0.5F)});
        node->addComponent<comp::Render>(Color(0x540099FF), Vector2(0.5F, 1.F));
        //======================================debug==============================
    }
} // namespace stay

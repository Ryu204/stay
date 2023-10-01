#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene()
        : mPhysicsWorld({0.F, -10.F})
        , mPixelsPerMeter(100.F)
        , mSceneLoader(&mManager, "asset/test.json")
    {
        Node::init(mManager.getRegistry());
        initialize();
    }
    Scene::~Scene()
    {
        mSceneRoot.reset();
        Node::shutdown();
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
        mManager.registerSystem<sys::RawRenderSystem>();
        //mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>()->initialize(&mPhysicsWorld);
        mManager.registerSystem<sys::PhysicsSystem>()->initialize(&mPhysicsWorld);
        
        mSceneLoader.registerType<comp::Render>("render");
        mSceneLoader.registerType<phys::Collider>("collider");
        mSceneLoader.registerType<phys::RigidBody>("rigidbody");
        mSceneRoot = mSceneLoader.load();
    }
} // namespace stay

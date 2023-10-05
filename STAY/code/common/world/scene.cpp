#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene(std::filesystem::path&& filepath)
        : mPhysicsWorld({0.F, -10.F})
        , mPixelsPerMeter(100.F)
        , mSceneLoader(&mManager, std::forward<std::filesystem::path>(filepath))
    {
        Node::init(mManager.getRegistry());
        initialize();
    }
    Scene::~Scene()
    {
        mSceneLoader.save(mSceneRoot.get());
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
        // mManager.registerSystem<sys::RawRenderSystem>();
        // mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>()->initialize(&mPhysicsWorld);
        mManager.registerSystem<sys::PhysicsSystem>()->initialize(&mPhysicsWorld);
        mManager.registerSystem<PlayerSystem>();
        
        mSceneLoader
            .registerComponent<comp::Render>("render")
            .registerComponent<phys::Collider>("collider")
            .registerComponent<phys::RigidBody>("rigidbody")
            .registerComponent<Player>("player");
        mSceneRoot = mSceneLoader.load();
    }
} // namespace stay

#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "../physics/world.hpp"
#include "camera.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene(std::filesystem::path&& filepath, RWin* window)
        : PIXELS_PER_METER(100.F)
        , mSceneLoader(&mManager, std::forward<std::filesystem::path>(filepath))
    {
        Node::init(mManager.getRegistry());
        phys::World::init();
        initialize(window);
    }
    Scene::~Scene()
    {
        mSceneLoader.save(mSceneRoot.get());
        mSceneRoot.reset();
        phys::World::shutdown();
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
    void Scene::initialize(RWin* window)
    {
        // mManager.registerSystem<sys::RawRenderSystem>();
        mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>()->initialize();
        mManager.registerSystem<sys::PhysicsSystem>()->initialize();
        mManager.registerSystem<PlayerSystem>();
        mManager.registerSystem<HookSystem>();
        mManager.registerSystem<DebugSystem>()->initialize(&mCamera, window);
        mManager.registerSystem<DashSystem>();
        
        mSceneLoader
            .registerComponent<comp::Render>("render")
            .registerComponent<phys::Collider>("collider")
            .registerComponent<phys::RigidBody>("rigidbody")
            .registerComponent<phys::RigidBody01>("rigidbody01")
            .registerComponent<phys::RigidBody02>("rigidbody02")
            .registerComponent<Player>("player")
            .registerComponent<PlayerDebug>("debug")
            .registerComponent<Hook>("hook")
            .registerComponent<phys::Joint>("joint")
            .registerComponent<phys::Collider01>("collider01")
            .registerComponent<phys::Collider02>("collider02")
            .registerComponent<Dash>("dash");
        mSceneRoot = mSceneLoader.load();
    }
} // namespace stay

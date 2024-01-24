#include "ecs/manager.hpp"
#include "physics/world.hpp"
#include "type/vector.hpp"
#include "../game/system/list.hpp"
#include "camera.hpp"
#include "node.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene(std::filesystem::path&& filepath, RWin* window)
        : PIXELS_PER_METER(100.F)
        , mSceneLoader(mManager, std::forward<std::filesystem::path>(filepath))
    {
        Node::init(mManager.getRegistry());
        phys::World::init(20.F * vectorDown);
        initialize(window);
    }
    Scene::~Scene()
    {
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
    void Scene::saveToFile()
    {
        mSceneLoader.save(mSceneRoot.get());
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

        mSceneRoot = mSceneLoader.load();
    }
} // namespace stay

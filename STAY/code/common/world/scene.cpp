#include "ecs/manager.hpp"
#include "physics/world.hpp"
#include "type/vector.hpp"
#include "../game/system/list.hpp"
#include "camera.hpp"
#include "node.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene(std::filesystem::path&& filepath, RWin& window)
        : mWindow(window)
        , mCamera(40.F)
        , mSceneLoader(mManager, std::forward<std::filesystem::path>(filepath))
        , PIXELS_PER_METER(100.F)
    {
        Node::init(mManager.getRegistry());
        phys::World::init(20.F * vectorDown);
        initialize();
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
        mManager.start(ecs::SystemContext{ mCamera, mWindow});
    }
    void Scene::saveToFile()
    {
        mSceneLoader.save(mSceneRoot.get());
    }
    void Scene::input(const sf::Event& event)
    {
        mManager.input(event);
    }
    void Scene::render(RTarget& target)
    {
        mCamera.setOn(&target);
        mManager.render(&target, mSceneRoot.get());
    }
    void Scene::initialize()
    {
        // mManager.registerSystem<sys::RawRenderSystem>();
        mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>();
        mManager.registerSystem<sys::PhysicsSystem>();
        mManager.registerSystem<PlayerSystem>();
        mManager.registerSystem<HookSystem>();
        mManager.registerSystem<DebugSystem>();
        mManager.registerSystem<DashSystem>();

        mSceneRoot = mSceneLoader.load();
    }
} // namespace stay

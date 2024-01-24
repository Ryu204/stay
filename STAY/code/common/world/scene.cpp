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
        , PIXELS_PER_METER(100.F)
    {
        phys::World::init(20.F * vectorDown);
        ecs::manager().reset(ecs::SystemContext{ mCamera, mWindow});
        Node::init(ecs::manager().getRegistry());
        mSceneLoader = std::make_unique<SceneLoader>(ecs::manager(), std::move(filepath));
        mSceneRoot = mSceneLoader->load();
    }
    Scene::~Scene()
    {
        mSceneRoot.reset();
        phys::World::shutdown();
        Node::shutdown();
    }
    void Scene::update(float dt)
    {
        ecs::manager().update(dt);
    }
    void Scene::start()
    {
        ecs::manager().start();
    }
    void Scene::saveToFile()
    {
        mSceneLoader->save(mSceneRoot.get());
    }
    void Scene::input(const sf::Event& event)
    {
        ecs::manager().input(event);
    }
    void Scene::render(RTarget& target)
    {
        mCamera.setOn(&target);
        ecs::manager().render(&target, mSceneRoot.get());
    }
} // namespace stay

#include "stay/ecs/manager.hpp"
#include "stay/physics/world.hpp"
#include "stay/type/vector.hpp"
#include "stay/world/camera.hpp"
#include "stay/world/node.hpp"
#include "stay/world/scene.hpp"

#include <fstream>

namespace stay
{
    Scene::Scene(std::filesystem::path&& filepath, RWin& window, Builtin systems)
        : mWindow{window}
        , mCamera{40.F}
        , mTextures{"asset/texture"}
        , PIXELS_PER_METER{100.F}
        , SYSTEMS_CONFIG_FILE{"asset/sysConfig.json"}
    {
        phys::World::init(20.F * vectorDown);
        ecs::manager().reset(
            ecs::SystemContext{mCamera, mWindow, mTextures},
            getSystemConfig(),
            systems
        );
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
    Serializable::Data Scene::getSystemConfig() const
    {
        std::ifstream reader{SYSTEMS_CONFIG_FILE};
        return nlohmann::json::parse(reader);
    }
} // namespace stay

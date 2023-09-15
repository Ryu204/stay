#include "node.hpp"
#include "../ecs/manager.hpp"
#include "camera.hpp"
#include "../../game/system/RawRenderSystem.hpp"
#include "scene.hpp"

namespace stay
{
    Scene::Scene(RTarget* target)
        : mCamera(Camera::defaultOfTarget(*target))
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
        target->setView(mCamera.getView());
        mManager.render(target);
    }
    void Scene::initialize()
    {
        mManager.registerSystem<sys::RawRenderSystem>();
        auto& reg = mManager.getRegistry();
        auto* node = create();
        reg.emplace<comp::Render>(node->getEntity(), sf::Color::Black, sf::Vector2f{40, 60});
        node->getLocalTransform().move({100, -100, 0});
        for (int i = 1; i <= 4; ++i)
        {
            node = createChild(node);
            reg.emplace<comp::Render>(node->getEntity(), sf::Color::Red, sf::Vector2f{70, 50});
            node->getLocalTransform().move({35, 35, 0});
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

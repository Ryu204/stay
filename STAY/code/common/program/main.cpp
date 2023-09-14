#include <iostream>

#include <json/json.h>

#include "application.hpp"
#include "../world/node.hpp"

int main()
{
    try
    {
        stay::program::Application app;
        app.run();
        return 0;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    catch(...)
    {
        std::cerr << "Unknown error!\n";
        return -2;
    }
}
// NOLINTBEGIN
// test==================================================================
int testNode()
{
    try
    {
        auto* node1 = stay::Node::root().createEmptyChild();
        auto* node2 = stay::Node::root().createEmptyChild();
        auto* node3 = stay::Node::create();

        node3->setParent(node1);
        node1->setParent(node2);
        std::cout << node3->isChildOf(node2) << node2->isParentOf(node3) << std::endl;
        stay::Node::destroy(node2);
        std::cout << "about to kill node2\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return -1;
}
// test==================================================================
#include "../ecs/manager.hpp"
#include <thread>
#include <chrono>
using namespace stay;
using namespace std::chrono_literals;

struct Physics : public ecs::System, public ecs::UpdateSystem, public ecs::StartSystem
{
    Physics(ecs::Manager* manager)
        : ecs::System(manager)
        , ecs::UpdateSystem{1}
        , ecs::StartSystem{1}
    {}

    void start() override
    {
        std::cout << "Physics system inited" << std::endl;
    }

    void update(float/*dt*/) override
    {
        std::cout << "Physics system updating" << std::endl;
    }
};

struct Render : public ecs::System, public ecs::StartSystem
{
    Render(ecs::Manager* managet)
        : ecs::System(managet)
        , ecs::StartSystem(-2)
    {}

    void start() override
    {
        std::cout << "render system inited" << std::endl;
    }
};

int systemRegistration()
{
    ecs::Manager manager;
    manager.registerSystem<Physics>();
    manager.registerSystem<Render>();

    manager.start();
    while (true)
    {
        manager.update(1);
        std::this_thread::sleep_for(1000ms);
        return 0;
    }
    return 1;
}
// test==================================================================
// NOLINTEND
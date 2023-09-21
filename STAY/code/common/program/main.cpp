#include <iostream>

#include <json/json.h>

#include "application.hpp"
#include "../world/node.hpp"

int main1()
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
#include <box2d/box2d.h>

int b2dHelloWorld()
{
    b2Vec2 gravity(0.F, -9.8F);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.F, -10.F);
    auto* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape box;
    box.SetAsBox(50.F, 10.F);

    groundBody->CreateFixture(&box, 0.F);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.F, 40.F);
    auto* body = world.CreateBody(&bodyDef);
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(1.F, 1.F);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.F;
    fixtureDef.friction = 0.3F;
    fixtureDef.shape = &bodyShape;
    body->CreateFixture(&fixtureDef);

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    sf::RectangleShape ground(sf::Vector2f{100.F, 20.F});
    sf::RectangleShape player(sf::Vector2f{10.F, 10.F});

    ground.setFillColor(sf::Color(0x008800FF));
    ground.setOutlineColor(sf::Color::Green);
    ground.setOutlineThickness(-1.F);
    ground.setPosition(0, -10);

    player.setFillColor(sf::Color(0x990000FF));
    player.setOutlineColor(sf::Color::Red);
    player.setOutlineThickness(-1.F);

    RWin win(sf::VideoMode(60, 60), "b2d");

    sf::Clock clock;
    float elapsed = 0.F;

for (int32 i = 0; i < 60; ++i)
{
    world.Step(1.F / 60.F, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

    while (win.isOpen())
    {
        elapsed += clock.restart().asSeconds();
        while (elapsed > 1.F / 60.F)
        {
            elapsed -= 1.F / 60.F;
            sf::Event e;
            while (win.pollEvent(e))
            {
                switch (e.type)
                {
                case sf::Event::Closed:
                    win.close();
                    break;
                default:
                    break;
                }
            }
            // update
            world.Step(1.F / 60.F, velocityIterations, positionIterations);
            player.setPosition(body->GetPosition().x, body->GetPosition().y);
            player.setRotation(body->GetAngle() * 180 / 3.14);
        }
        // render
        win.clear();
        win.draw(player);
        win.draw(ground);
        win.display();
    }

    return 0;
}
// test==================================================================
#include "../event/event.hpp"
using namespace stay::event;

Event<int, float> compiling;
int id;

void outOfScope()
{
    std::string source = "foo.cpp";
    std::string obj = "foo.obj";
    id = compiling += [&,source](int files, float time){
        std::cout << "GNU compiling " << files << " files including " << source << ", time elapsed: " << time << "s" << std::endl;
    };
}

int main()
{
    outOfScope();
    compiling.invoke(2, 0.5F);
    compiling -= id;
    compiling.invoke(100, 100.F);
}
// test==================================================================
// NOLINTEND
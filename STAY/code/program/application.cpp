#include <fstream>
#include <iostream>

#include "json/json.h"

#include "application.hpp"
#include "../type/vector.hpp"

///testing
// NOLINTBEGIN
#include <SFML/Graphics.hpp>
#include "../world/transform.hpp"
#include "../world/node.hpp"
#include "../utility/convert.hpp"
namespace 
{
    sf::RectangleShape shape(sf::Vector2f(30, 30));
    stay::Node& node1 = *stay::Node::create();
    stay::Node& node2 = *stay::Node::create();
    void init(stay::RWin* window)
    {
        node1.setParent(&stay::Node::root());
        node2.setParent(&node1);

        shape.setOrigin(15, 15);
        shape.setFillColor(sf::Color::Red);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(2.F);
        auto newView = window->getView();
        newView.setCenter(0, 0);
        window->setView(newView);
    }
    void drawShape(stay::RWin* win, int id)
    {
        if (id == 1)
        {
            shape.setFillColor(sf::Color::White);
            auto trans = node1.getGlobalTransform();
            auto pos = trans.getPosition();
            auto scale = trans.getScale();
            shape.setPosition(pos.x, -pos.y);
            shape.setScale(scale.x, scale.y);
            shape.setRotation(trans.getRotation());
            win->draw(shape);
        }
        else if (id == 2)
        {
            shape.setFillColor(sf::Color::Cyan);
            auto trans = node2.getGlobalTransform();
            auto pos = trans.getPosition();
            auto scale = trans.getScale();
            shape.setPosition(pos.x, -pos.y);
            shape.setScale(scale.x, scale.y);
            shape.setRotation(trans.getRotation());
            win->draw(shape);
        }
    }
    void updateaslkfj(float dt)
    {
        stay::Vector2 vel{0, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            vel.x += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            vel.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            vel.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            vel.y -= 1;
        node1.getLocalTransform().move(300 * dt * vel);
        stay::Vector3 scale{1.F, 1.F, 1.F};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            scale.x = 1.1F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
            scale.y = 1.1F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            scale.z = 1.1F;
        node1.getLocalTransform().scale(scale);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        node1.getLocalTransform().setScale({1, 1, 1});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        node1.getLocalTransform().rotate(-50 * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        node1.getLocalTransform().rotate(50 * dt);

        vel = stay::Vector2{0, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
            vel.x += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
            vel.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
            vel.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
            vel.y -= 1;
        node2.getLocalTransform().move(300 * dt * vel);
        scale = stay::Vector3{1.F, 1.F, 1.F};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
            scale.x = 1.1F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
            scale.y = 1.1F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
            scale.z = 1.1F;
        node2.getLocalTransform().scale(scale);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        node2.getLocalTransform().setScale({1, 1, 1});
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        node2.getLocalTransform().rotate(-50 * dt);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        node2.getLocalTransform().rotate(50 * dt);
    }
    //NOLINTEND
} // namespace
//

namespace stay
{
    namespace program
    {
        Application::Application()
        {
            initialize();
            init(mWindow.get());
        }

        Application::~Application()
        {
            saveProperties();
        }

        void Application::initialize()
        {
            // Fetch data from file
            std::ifstream dataReader(INIT_FILE);        
            Json::Value data;
            dataReader >> data;
            dataReader.close();
            mAppInfo.fetch(data["window"]);

            // Initialization
            mWindow = std::make_unique<RWin>(sf::VideoMode(mAppInfo.width, mAppInfo.height), mAppInfo.name);
        }

        void Application::saveProperties()
        {
            // Update
            auto winSize = mWindow->getSize();
            mAppInfo.width = winSize.x;
            mAppInfo.height = winSize.y;

            // Write
            Json::Value root;
            root["window"] = mAppInfo.toJSONObject();
            std::ofstream dataWriter(INIT_FILE);
            dataWriter << root;
        }

        void Application::run()
        {
            sf::Clock clock;
            float queuedTime = 0.F;
            float timePerUpdate = 1.F / mAppInfo.updatesPerSec;

            while (mWindow->isOpen())
            {
                queuedTime += clock.restart().asSeconds();
                while (queuedTime >= timePerUpdate)
                {
                    input();
                    update();
                    updateaslkfj(timePerUpdate);
                    queuedTime -= timePerUpdate;
                }
                render();
            }
        }

        void Application::input()
        {
            sf::Event event;
            while (mWindow->pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    mWindow->close();
                    break;
                default:
                    break;
                }
            }
        }

        void Application::update()
        {

        }

        void Application::render()
        {
            mWindow->clear(sf::Color(0x1144aaff));

            drawShape(mWindow.get(), 1);
            drawShape(mWindow.get(), 2);

            mWindow->display();
        }

    } // namespace program
} // namespace stay

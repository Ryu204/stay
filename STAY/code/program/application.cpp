#include <fstream>
#include <iostream>

#include "json/json.h"

#include "application.hpp"
#include "../type/vector.hpp"

namespace stay
{
    namespace program
    {
        Application::Application()
        {
            initialize();
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

            mScene = std::make_unique<Scene>(mWindow.get());
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

            mScene->start();

            while (mWindow->isOpen())
            {
                queuedTime += clock.restart().asSeconds();
                while (queuedTime >= timePerUpdate)
                {
                    input();
                    update(timePerUpdate);
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
                mScene->input(event);
            }
        }

        void Application::update(float dt)
        {
            mScene->update(dt);
        }

        void Application::render()
        {
            mWindow->clear(sf::Color(0x1144aaff));
            mScene->render(mWindow.get());
            mWindow->display();
        }
    } // namespace program
} // namespace stay

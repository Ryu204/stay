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
            : mSprite(sf::TrianglesStrip, 4)
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

            setUpRendering();

            mScene = std::make_unique<Scene>(mTexture.get());
        }

        void Application::setUpRendering()
        {
            mWindow = std::make_unique<RWin>(sf::VideoMode(mAppInfo.width, mAppInfo.height), mAppInfo.name);
            auto view = mWindow->getView();
            view.setCenter(sf::Vector2f());
            mWindow->setView(view);
            auto sizeu = mWindow->getSize();
            auto sizef = sf::Vector2f{sizeu};
            mTexture = std::make_unique<RTexture>();
            mTexture->create(sizeu.x, sizeu.y);
            mSprite[0].position.x = -sizef.x / 2.F;      mSprite[0].position.y = -sizef.y / 2.F;
            mSprite[1].position.x = sizef.x / 2.F;       mSprite[1].position.y = -sizef.y / 2.F;
            mSprite[2].position.x = -sizef.x / 2.F;      mSprite[2].position.y = sizef.y / 2.F;
            mSprite[3].position.x = sizef.x / 2.F;       mSprite[3].position.y = sizef.y / 2.F;
            mSprite[0].texCoords.x = 0.F;                 mSprite[0].texCoords.y = sizef.y;
            mSprite[1].texCoords.x = sizef.x;            mSprite[1].texCoords.y = sizef.y;
            mSprite[2].texCoords.x = 0.F;                 mSprite[2].texCoords.y = 0.F;
            mSprite[3].texCoords.x = sizef.x;            mSprite[3].texCoords.y = 0.F;
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
            mWindow->clear();
            mTexture->clear(sf::Color::Green);
            // Start drawing here
            mScene->render();
            // End drawing
            mTexture->display();
            RStates states;
            states.texture = &(mTexture->getTexture());
            mWindow->draw(mSprite, states);
            mWindow->display();
        }
    } // namespace program
} // namespace stay

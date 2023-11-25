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
            if (!dataReader.good())       
                throw std::runtime_error("Cannot open init file");
            Json::Value data;
            dataReader >> data;
            dataReader.close();
            mAppInfo.fetch(data["window"]);
            
            mWindow = std::make_unique<RWin>(sf::VideoMode(mAppInfo.width, mAppInfo.height), mAppInfo.name);
            mWindow->setKeyRepeatEnabled(false);
            setUpRendering();

            mScene = std::make_unique<Scene>(data["scene"].asString(), mWindow.get());
        }

        void Application::setUpRendering()
        {
            auto sizeu = mWindow->getSize();
            auto sizef = sf::Vector2f{sizeu};
            auto view = sf::View(sf::Vector2f(), sizef);
            mWindow->setView(view);
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
            // Read file (to keep existing data)
            Json::Value root;
            std::ifstream(INIT_FILE) >> root;
            // Write
            root["window"] = mAppInfo.toJSONObject();
            std::ofstream dataWriter(INIT_FILE);
            dataWriter << root;
        }

        void Application::run()
        {
            sf::Clock clock;
            float queuedTime = 0.F;
            const float timePerUpdate = 1.F / mAppInfo.updatesPerSec;

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
                case sf::Event::Resized:
                    setUpRendering();
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
            mTexture->clear();
            // Start drawing here
            mScene->render(mTexture.get());
            // End drawing
            mTexture->display();
            RStates states;
            states.texture = &(mTexture->getTexture());
            mWindow->draw(mSprite, states);
            mWindow->display();
        }
    } // namespace program
} // namespace stay

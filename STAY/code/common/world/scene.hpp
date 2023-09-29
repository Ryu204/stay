#pragma once

#include <box2d/box2d.h>

#include "../ecs/manager.hpp"
#include "camera.hpp"
#include "../loader/sceneLoader.hpp"

namespace stay
{
    class Scene
    {
        public:
            Scene();
            ~Scene();
            void update(float dt);
            void start();
            void input(const sf::Event& event);
            void render(RTarget* target);
        private:
            void initialize();

            Uptr<Node> mSceneRoot;

            Camera mCamera;
            ecs::Manager mManager;

            b2World mPhysicsWorld;
            const float mPixelsPerMeter;
            //SceneLoader mSceneLoader;
    };
} // namespace stay

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
            b2World mPhysicsWorld;
            Camera mCamera;
            ecs::Manager mManager;
            Uptr<Node> mSceneRoot;
            SceneLoader mSceneLoader;

            const float mPixelsPerMeter;
    };
} // namespace stay

#pragma once

#include "node.hpp"
#include "../ecs/manager.hpp"
#include "camera.hpp"

namespace stay
{
    class Scene
    {
        public:
            Scene(RTarget* target);
            ~Scene();
            void update(float dt);
            void start();
            void input(const sf::Event& event);
            void render(RTarget* target);
        private:
            void initialize();
            Node* create();
            Node* createChild(Node* node);

            Camera mCamera;
            ecs::Manager mManager;
    };
} // namespace stay

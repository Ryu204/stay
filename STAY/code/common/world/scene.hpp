#pragma once

#include "node.hpp"
#include "../ecs/manager.hpp"
#include "camera.hpp"

namespace stay
{
    class Scene
    {
        public:
            Scene(RTarget* target)
                : mCamera(Camera::defaultOfTarget(*target))
            {
                Node::root().assign(&mManager);
            }
            ~Scene()
            {
                Node::root().clearChildren();
            }
            void update(float dt)
            {
                mManager.update(dt);
            }
            void start()
            {
                mManager.start();
            }
            void input(const sf::Event& event)
            {
                mManager.input(event);
            }
            void render(RTarget* target)
            {
                target->setView(mCamera.getView());
                mManager.render(target);
            }
        private:
            Camera mCamera;
            ecs::Manager mManager;
    };
} // namespace stay

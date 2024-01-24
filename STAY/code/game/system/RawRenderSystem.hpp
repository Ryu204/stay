#pragma once

#include "ecs/manager.hpp"
#include "../component/render.hpp"
#include "utility/convert.hpp"
#include "utility/sfutils.hpp"

/*
    This system renders by iterating through all object with `render` component and use it global transform
*/

namespace stay
{
    namespace sys
    {
        struct RawRenderSystem : public ecs::RenderSystem, public ecs::System
        {
                RawRenderSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(0)
                    , ecs::System(manager)
                {
                    mShape.setOutlineColor(sf::Color::White);
                    mShape.setOutlineThickness(0.1);
                }

                void render(RTarget* target, Node* /*root*/) override
                {
                    auto view = mManager->getRegistryRef().view<comp::Render>();
                    for (auto entity : view)
                    {
                        auto& render = mManager->getRegistryRef().get<comp::Render>(entity);
                        auto* node = Node::getNode(entity);
                        const auto tf = node->globalTransform();
                        mShape.setSize(utils::convertVec2<sf::Vector2f>(render.size));
                        mShape.setPosition(utils::convertVec2<sf::Vector2f>(tf.getPosition()));
                        utils::centerSf(mShape);
                        mShape.setFillColor(render.color);
                        target->draw(mShape);
                    }
                }

            private:
                sf::RectangleShape mShape;
        };
    } // namespace sys
} // namespace stay

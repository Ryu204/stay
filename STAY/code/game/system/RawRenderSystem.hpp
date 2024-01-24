#pragma once

#include "ecs/manager.hpp"
#include "../component/render.hpp"
#include "utility/convert.hpp"
#include "utility/sfutils.hpp"
#include <SFML/System/Vector2.hpp>

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
                        mShape.setSize(render.size.toVec2<sf::Vector2f>());
                        mShape.setPosition(tf.getPosition().toVec2<sf::Vector2f>());
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

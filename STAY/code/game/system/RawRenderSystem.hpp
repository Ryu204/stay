#pragma once

#include "../../common/ecs/manager.hpp"
#include "../component/render.hpp"
#include "../../common/utility/convert.hpp"
#include "../../common/utility/sfutils.hpp"

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
                    mShape.setSize({50, 50});
                    mShape.setOutlineColor(sf::Color::White);
                    mShape.setOutlineThickness(2);
                    utils::centersf(mShape);
                }

                void render(RTarget* target) override
                {
                    auto view = mManager->getRegistry().view<comp::Render>();
                    for (auto [entity, render] : view.each())
                    {
                        auto* node = Node::getNode(entity);
                        const auto& tf = node->getGlobalTransform();
                        mShape.setSize(render.size);
                        utils::centersf(mShape);
                        mShape.setFillColor(render.color);
                        RStates states = RStates::Default;
                        states.transform *= utils::transTosfTrans(tf);
                        target->draw(mShape, states);
                    }
                }

            private:
                sf::RectangleShape mShape;
        };
    } // namespace sys
} // namespace stay

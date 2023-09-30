#pragma once

#include "../component/render.hpp"
#include "../../common/ecs/manager.hpp"
#include "../../common/utility/convert.hpp"
#include "../../common/utility/sfutils.hpp"

/*
    Render the whole scene by traversing every node using BFS
*/

namespace stay
{
    namespace sys
    {
        struct OrderedRenderSystem : public ecs::RenderSystem, public ecs::System
        {
                OrderedRenderSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(0)
                    , ecs::System(manager)
                {
                    mShape.setOutlineColor(sf::Color::White);
                    mShape.setOutlineThickness(0.1F);
                    utils::centersf(mShape);
                }

                void render(RTarget* target, Node* root) override
                {
                    drawOn(root, target);
                }
            private:
                // Draw `node` and its children on target with current camera config
                void drawOn(Node* node, RTarget* target)
                {
                    assert(node != nullptr && "call on nullptr");
                    
                    const auto draw = [&target, this](Node* current, RStates states) -> RStates
                    {
                        // Make the get function const because `current` can be the root
                        const auto& tf = ((const Node*)current)->localTransform();
                        states.transform = states.transform * utils::transTosfTrans(tf);
                        if (current->hasComponent<comp::Render>())
                        {
                            const auto& drawable = current->getComponent<comp::Render>();
                            mShape.setSize(utils::convertVec2<sf::Vector2f>(drawable.size));
                            utils::centersf(mShape);
                            mShape.setFillColor(drawable.color);
                            target->draw(mShape, states);
                        }
                        return states;
                    };
                    node->visitChained(draw, RStates::Default);
                }

                sf::RectangleShape mShape;
        };
    } // namespace sys
} // namespace stay

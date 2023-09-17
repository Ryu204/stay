#pragma once

#include "../../common/ecs/manager.hpp"
#include "../component/render.hpp"
#include "../../common/world/node.hpp"
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
                    mShape.setSize({50, 50});
                    mShape.setOutlineColor(sf::Color::White);
                    mShape.setOutlineThickness(2);
                    utils::centersf(mShape);
                }

                void render(RTarget* target) override
                {
                    drawOn(&Node::root(), target);
                }
            private:
                // Draw `node` and its children on target with current camera config
                void drawOn(Node* node, RTarget* target)
                {
                    assert(node != nullptr && "call on nullptr");
                    
                    const auto draw = [&target, this](Node* current, RStates states) -> RStates
                    {
                        const auto& tf = ((const Node*)current)->getLocalTransform();
                        states.transform = states.transform * utils::transTosfTrans(tf);
                        if (current->hasComponent<comp::Render>())
                        {
                            const auto& drawable = current->getComponent<comp::Render>();
                            mShape.setSize(drawable.size);
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
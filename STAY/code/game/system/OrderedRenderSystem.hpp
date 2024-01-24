#pragma once

#include <SFML/System/Vector2.hpp>

#include "../component/render.hpp"
#include "ecs/system.hpp"
#include "utility/convert.hpp"
#include "utility/sfutils.hpp"

/*
    Render the whole scene by traversing every node using BFS
*/

namespace stay
{
    namespace sys
    {
        struct OrderedRenderSystem : public ecs::RenderSystem, public ecs::System
        {
                REGISTER_SYSTEM(OrderedRenderSystem)
                OrderedRenderSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(0)
                    , ecs::System(manager)
                {
                    mShape.setOutlineColor(sf::Color::White);
                    mShape.setOutlineThickness(0.1F);
                    utils::centerSf(mShape);
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
                        const auto& tf = current->localTransform();
                        states.transform = states.transform * utils::transTosfTrans(tf);
                        if (current->hasComponent<comp::Render>())
                        {
                            const auto& drawable = current->getComponent<comp::Render>();
                            mShape.setSize(drawable.size.toVec2<sf::Vector2f>());
                            utils::centerSf(mShape);
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

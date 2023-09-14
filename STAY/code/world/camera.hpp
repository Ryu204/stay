#pragma once

#include <SFML/Graphics.hpp>

#include "../type/vector.hpp"
#include "node.hpp"
#include "../utility/convert.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    class Camera
    {
        public:
            using View = sf::View;

            static Camera defaultOfTarget(const RTarget& target)
            {
                auto size = target.getSize();
                return Camera(Vector2(), Vector2(size.x, size.y));
            }
            
            // NOLINTBEGIN(performance-unnecessary-value-param)
            Camera(Vector2 center, Vector2 size)
                : mView(utils::vecTosfVec(center), utils::vecTosfVec(size))
            { }
            // NOLINTEND(performance-unnecessary-value-param)

            Camera(View view)
                : mView(view)
            { }

            View& getView()
            {
                return mView;
            }

            // Draw `node` and its children on target with current camera config
            RStates drawOn(Node* node, RTarget* target)
            {
                assert(node != nullptr && "call on nullptr");
                
                const auto draw = [&target, this](Node* current, RStates states) -> RStates
                {
                    states.transform = states.transform * utils::transTosfTrans(current->getLocalTransform());
                    auto* drawable = dynamic_cast<sf::Drawable*>(current);
                    if (drawable != nullptr)
                    {
                        target->draw(*drawable, states);
                    }
                    return states;
                };

                target->setView(mView);
                node->visitChained(draw, RStates::Default);
            }
        private:
            View mView;
    };
} // namespace stay

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
        private:
            View mView;
    };
} // namespace stay

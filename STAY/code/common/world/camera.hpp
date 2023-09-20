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
            
            Camera(const Vector2& center, const Vector2& size)
                : mView(utils::convertVec2<sf::Vector2f>(center), utils::convertVec2<sf::Vector2f>(size))
            { }

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

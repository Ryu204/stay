#pragma once

#include <SFML/Graphics.hpp>

#include "stay/type/vector.hpp"
#include "stay/utility/convert.hpp"
#include "stay/utility/typedef.hpp"

namespace stay
{
    class Camera
    {
        public:
            using View = sf::View;
            
            Camera(float height = 40.F)
                : mHeight(height)
                , mView(sf::Vector2f(0.F, 0.F), sf::Vector2f())
            { 
                adaptTo(Vector2(1.F, 1.F));
            }

            // @return A reference to internal `sf::View` for modification
            // @note If you change the scale, it won't be automatically recomputed. Use `adaptTo(...)` to recompute the right ratio
            View& getView()
            {
                return mView;
            }
            // @brief Adapt to `target` ratio and modify its view to match this camera
            void setOn(RTarget* target)
            {
                adaptTo(target);
                target->setView(mView);
            }
            // @brief Change camera's aspect ratio to fit into `target` while keeping internal height
            void adaptTo(const RTarget* target)
            {
                adaptTo(Vector2::from(target->getSize()));
            }
            // @brief Adjust aspect ratio to match `size` and the height is retanined
            void adaptTo(const Vector2& size)
            {
                mView.setSize(sf::Vector2f(mHeight * size.x / size.y, mHeight));
            }
        private:
            // Height of the visible area in meters
            float mHeight;
            View mView;
    };
} // namespace stay

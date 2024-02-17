#pragma once

#include "serializable.hpp"
#include "vector.hpp"

namespace stay
{
    struct Rect : Serializable
    {
            Rect(Vector2 min = Vector2{}, Vector2 max = Vector2{})
                : mMin{std::move(min)}
                , mMax{std::move(max)}
            {
                if (mMin.x > mMax.x)
                    std::swap(mMin.x, mMax.x);
                if (mMin.y > mMax.y)
                    std::swap(mMin.y, mMax.y);
            }     
            Vector2 center() const 
            {
                return (mMin + mMax) / 2.F;
            }  
            Vector2 size() const
            {
                return mMax - mMin;
            }
            Vector2 min() const 
            {
                return mMin;
            }
            Vector2 max() const
            {
                return mMax;
            }
            void setMin(const Vector2& min)
            {
                assert(min.x <= mMax.x && min.y <= mMax.y && "invalid value");
                mMin = min;
            }
            void setMax(const Vector2& max)
            {
                assert(max.x >= mMin.x && max.y >= mMin.y && "invalid value");
                mMax = max;
            }
            SERIALIZE(mMin, mMax)
        private:
            Vector2 mMin;
            Vector2 mMax;
    };
} // namespace stay

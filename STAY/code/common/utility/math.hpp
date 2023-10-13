#pragma once

namespace stay
{
    namespace utils
    {
        template <typename T>
        int mostSignificantBit(T n)
        {
            if (n == static_cast<T>(0))
                return -1;
            T ret;                                                                      
            do {                                                                        
                ret = n;                                                                
                n &= n - 1;                                                             
            } while (n);                                                                
            int res = 0;
            while (ret != static_cast<T>(1))
            {
                res++;
                ret /= 2;
            }
            return res;
        }
        template <typename T>
        float lengthVec2(const T& t)
        {
            return std::sqrt(t.x * t.x + t.y * t.y);
        }
    } // namespace utils
} // namespace stay

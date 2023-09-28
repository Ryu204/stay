#pragma once

#include <exception>
#include <cassert>

namespace stay
{
    namespace utils
    {
        template <typename Cond, typename Mes>
        void throwIfFalse(Cond&& cond, const Mes& message)
        {
            if (static_cast<bool>(cond) == false)
                throw std::runtime_error(message);
        }
    } // namespace utils
} // namespace stay

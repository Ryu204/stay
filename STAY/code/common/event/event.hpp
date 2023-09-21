#pragma once

#include <vector>
#include <functional>

#include "../utility/IDgen.hpp"

/*
    Low effort on making a C++ event system like that in C#
*/

namespace stay
{
    namespace event
    {
        template <typename... EventArgs>
        struct Event
        {
                using Handler = std::function<void(EventArgs...)>;
                
                template <typename Func>
                std::size_t operator += (Func&& action)
                {
                    auto newID = mIDGen.generate();
                    mSubcribers.emplace(static_cast<int>(newID), (
                        [action = std::forward<Func>(action)](EventArgs&&... args)
                        {
                            action(std::forward<EventArgs>(args)...);
                        }
                    ));
                    return newID;
                }
                void operator -= (std::size_t index)
                {
                    mIDGen.erase(index);
                    mSubcribers.erase(static_cast<int>(index));
                }
                void invoke(EventArgs&&... args) const
                {
                    for (const auto& action : mSubcribers)
                    {
                        action.second(std::forward<EventArgs>(args)...);
                    }
                }
            private:
                std::map<int, Handler> mSubcribers;
                utils::IDGenerator<100> mIDGen;
        };
    } // namespace event
} // namespace stay

#pragma once

#include <unordered_map>
#include <functional>
#include <cstdint>

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
                
                // @return The index of the listener. This index is used to later remove the listener from invoke list if needed.
                template <typename Func>
                std::size_t addEventListener(Func&& action)
                {
                    auto newID = mIDGen.generate();
                    mSubcribers.emplace(newID,
                        [action = std::forward<Func>(action)](EventArgs&&... args)
                        {
                            action(std::forward<EventArgs>(args)...);
                        }
                    );
                    return newID;
                }
                void removeListener(std::size_t index)
                {
                    mIDGen.erase(index);
                    mSubcribers.erase(index);
                }
                void invoke(EventArgs&&... args) const
                {
                    for (const auto& action : mSubcribers)
                    {
                        action.second(std::forward<EventArgs>(args)...);
                    }
                }
            private:
                std::unordered_map<std::size_t, Handler> mSubcribers;
                utils::IDGenerator<100> mIDGen;
        };
    } // namespace event
} // namespace stay

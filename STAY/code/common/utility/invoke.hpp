#pragma once

#include <queue>
#include <cassert>
#include <functional>

#include "utility/IDgen.hpp"

namespace stay 
{
    // @brief Used to invoke a function after an interval
    // @note Does not keep track of time, must provide an interval each time to make progression.
    //       The accuracy depends on how small the provided interval is
    class Invoke
    {
        public:
            // @return ID of the callback (in case of removal)
            template <typename Func>
            static std::size_t after(float seconds, Func&& callback, bool repeated = false)
            {
                assert(seconds > 0.F && "Only future invocation is allowed");
                const auto timePoint = data().currentTime + seconds;
                const auto id = data().generator.generate();
                data().callbackData.emplace(Registry{
                        std::forward<Func>(callback),
                        seconds,
                        timePoint,
                        id,
                        repeated
                });
                return id;
            }
            static void removeCallback(std::size_t id);
            static void progress(float dt);
            static void reset();
        private:
            struct Registry
            {
                std::function<void()> callback{};
                float interval{0.5F};
                float timePoint{0.F};
                std::size_t id{0};
                bool repeated{false};
                bool operator < (const Registry& right) const;
            };

            struct Data 
            {
                float currentTime{0.F};
                std::priority_queue<Registry> callbackData{};
                utils::IdGenerator generator;
                void reset();
            };

            static Data& data();
    };
} // namespace stay
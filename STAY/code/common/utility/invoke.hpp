#pragma once

#include <queue>
#include <cassert>
#include <functional>

namespace stay 
{
    // @brief Used to invoke a function after an interval
    // @note Does not keep track of time, must provide an interval each time to make progression.
    //       The accuracy depends on how small the provided interval is
    class Invoke
    {
        public:
            template <typename Func>
            static void after(float seconds, Func&& callback)
            {
                assert(seconds > 0.F && "Only future invocation is allowed");
                data().callbackData.emplace(data().currentTime + seconds, std::forward<Func>(callback));
            }

            static void progress(float dt);
            // Delete current queueing callbacks
            static void terminate();
        private:
            struct Registry
            {
                Registry(float timePoint, std::function<void()>&& func);
                std::function<void()> callback{};
                float timePoint{0};
                bool operator < (const Registry& right) const;
            };

            struct Data 
            {
                float currentTime{0.F};
                std::priority_queue<Registry> callbackData{};
                void reset();
            };

            static Data& data();
    };
} // namespace stay
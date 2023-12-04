#include "invoke.hpp"
#include <queue>

namespace stay 
{
    void Invoke::progress(float dt)
    {
        data().currentTime += dt;
        auto& list = data().callbackData;
        while (!list.empty())
        {
            const auto& registry = list.top();
            if (registry.timePoint < data().currentTime)
            {
                registry.callback();
                list.pop();
            }
            else
                break;
        }
    }

    void Invoke::terminate()
    {
        data().reset();
    }

    Invoke::Registry::Registry(float timePoint, std::function<void()>&& func)
        : callback{func}
        , timePoint(timePoint)
    {}

    bool Invoke::Registry::operator < (const Registry& right) const 
    {
        // Smallest time point on top
        return timePoint > right.timePoint;
    }

    void Invoke::Data::reset()
    {
        currentTime = 0.F;
        std::priority_queue<Registry>().swap(callbackData);
    }

    Invoke::Data& Invoke::data()
    {
        static Data res;
        return res;
    }
} // namespace stay
#include "invoke.hpp"

namespace stay 
{
    void Invoke::removeCallback(std::size_t id)
    {
        assert(data().generator.isActive(id) && "id not added");
        data().generator.erase(id);
    }

    void Invoke::progress(float dt)
    {
        data().currentTime += dt;
        auto& list = data().callbackData;
        while (!list.empty())
        {
            const auto& registry = list.top();
            if (registry.timePoint < data().currentTime)
            {
                auto info = registry;
                const auto removed = !data().generator.isActive(info.id);
                list.pop();
                if (removed)
                    continue;
                if (info.repeated)
                {
                    info.timePoint += info.interval;
                    list.push(info);
                }
                info.callback();
            }
            else
                break;
        }
    }

    void Invoke::reset()
    {
        data().reset();
    }

    bool Invoke::Registry::operator < (const Registry& right) const 
    {
        // Smallest time point on top
        return timePoint > right.timePoint;
    }

    void Invoke::Data::reset()
    {
        currentTime = 0.F;
        std::priority_queue<Registry>().swap(callbackData);
        utils::IdGenerator().swap(generator);
    }

    Invoke::Data& Invoke::data()
    {
        static Data res;
        return res;
    }
} // namespace stay
#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "system.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    namespace ecs
    {
        namespace detail
        {
            template <typename T>
            struct Ordered
            {
                int id {};
                T val {};
            };
            template <typename T>
            struct Cmpr
            {
                bool operator () (const Ordered<T>& sml, const Ordered<T>& lgr)
                {
                    return sml.id < lgr.id;
                }
            };
        } // namespace detail
        
        class Manager 
        {
            public:
                Registry& getRegistry()
                {
                    return mRegistry;
                }
                const Registry& getRegistry() const
                {
                    return mRegistry;
                }
                
                template <typename DerviedSystem>
                SPtr<DerviedSystem> registerSystem()
                {
                    SPtr<DerviedSystem> ptr = std::make_shared<DerviedSystem>(this);
                    
                    SPtr<StartSystem> startPtr = std::dynamic_pointer_cast<StartSystem>(ptr);
                    if (startPtr.get() != nullptr)
                        mStartSystems.push_back(Pair<StartSystem>{ startPtr->orderStart, startPtr });

                    SPtr<UpdateSystem> updatePtr = std::dynamic_pointer_cast<UpdateSystem>(ptr);
                    if (updatePtr.get() != nullptr)
                        mUpdateSystems.push_back(Pair<UpdateSystem>{updatePtr->orderUpdate, updatePtr});

                    return ptr;
                }

                // Meant to be called only once, before any update
                void start()
                {
                    // In the same category, system with smallest id gets called first and so on
                    std::sort(mStartSystems.begin(), mStartSystems.end(), detail::Cmpr<SPtr<StartSystem>>());
                    std::sort(mUpdateSystems.begin(), mUpdateSystems.end(), detail::Cmpr<SPtr<UpdateSystem>>());

                    for (auto& pair : mStartSystems)
                    {
                        pair.val->start();
                    }
                }

                // Meant to be call every frame update
                void update(float dt)
                {
                    for (auto& pair : mUpdateSystems)
                    {
                        pair.val->update(dt);
                    }
                }

            private:
                template <typename T>
                using Pair = detail::Ordered<SPtr<T>>;
                Registry mRegistry{};
                std::vector<Pair<StartSystem>> mStartSystems;
                std::vector<Pair<UpdateSystem>> mUpdateSystems;
        };
    }
} // namespace stay

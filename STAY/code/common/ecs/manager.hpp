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
                    // Start
                    SPtr<StartSystem> startPtr = std::dynamic_pointer_cast<StartSystem>(ptr);
                    if (startPtr.get() != nullptr)
                        mStartSystems.push_back(Pair<StartSystem>{ startPtr->orderStart, startPtr });
                    // Update
                    SPtr<UpdateSystem> updatePtr = std::dynamic_pointer_cast<UpdateSystem>(ptr);
                    if (updatePtr.get() != nullptr)
                        mUpdateSystems.push_back(Pair<UpdateSystem>{updatePtr->orderUpdate, updatePtr});
                    // LateUpdate
                    SPtr<LateUpdateSystem> lateUpdatePtr = std::dynamic_pointer_cast<LateUpdateSystem>(ptr);
                    if (lateUpdatePtr.get() != nullptr)
                        mLateUpdateSystems.push_back(Pair<LateUpdateSystem>{lateUpdatePtr->orderLateUpdate, lateUpdatePtr});
                    // Render
                    SPtr<RenderSystem> renderPtr = std::dynamic_pointer_cast<RenderSystem>(ptr);
                    if (renderPtr.get() != nullptr)
                        mRenderSystems.push_back(Pair<RenderSystem>{renderPtr->orderRender, renderPtr});
                    // Input
                    SPtr<InputSystem> inputPtr = std::dynamic_pointer_cast<InputSystem>(ptr);
                    if (inputPtr.get() != nullptr)
                        mInputSystems.push_back(Pair<InputSystem>{inputPtr->orderInput, inputPtr});

                    return ptr;
                }

                // Meant to be called only once, before any update
                void start()
                {
                    // In the same category, system with smallest id gets called first and so on
                    std::sort(mStartSystems.begin(), mStartSystems.end(), detail::Cmpr<SPtr<StartSystem>>());
                    std::sort(mUpdateSystems.begin(), mUpdateSystems.end(), detail::Cmpr<SPtr<UpdateSystem>>());
                    std::sort(mLateUpdateSystems.begin(), mLateUpdateSystems.end(), detail::Cmpr<SPtr<LateUpdateSystem>>());
                    std::sort(mRenderSystems.begin(), mRenderSystems.end(), detail::Cmpr<SPtr<RenderSystem>>());
                    std::sort(mInputSystems.begin(), mInputSystems.end(), detail::Cmpr<SPtr<InputSystem>>());

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
                    for (auto& pair : mLateUpdateSystems)
                    {
                        pair.val->update(dt);
                    }
                }

                void render(RTarget* target)
                {
                    for (auto& pair : mRenderSystems)
                    {
                        pair.val->render(target);
                    }
                }

                void input(const sf::Event& event)
                {
                    for (auto& pair : mInputSystems)
                    {
                        pair.val->input(event);
                    }
                }

            private:
                template <typename T>
                using Pair = detail::Ordered<SPtr<T>>;
                Registry mRegistry{};
                std::vector<Pair<StartSystem>> mStartSystems{};
                std::vector<Pair<UpdateSystem>> mUpdateSystems{};
                std::vector<Pair<LateUpdateSystem>> mLateUpdateSystems{};
                std::vector<Pair<RenderSystem>> mRenderSystems{};
                std::vector<Pair<InputSystem>> mInputSystems{};
        };
    }
} // namespace stay

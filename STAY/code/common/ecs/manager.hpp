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
                    // PostUpdate
                    SPtr<PostUpdateSystem> lateUpdatePtr = std::dynamic_pointer_cast<PostUpdateSystem>(ptr);
                    if (lateUpdatePtr.get() != nullptr)
                        mPostUpdateSystems.push_back(Pair<PostUpdateSystem>{lateUpdatePtr->orderLateUpdate, lateUpdatePtr});
                    // PreUpdate
                    SPtr<PreUpdateSystem> preUpdatePtr = std::dynamic_pointer_cast<PreUpdateSystem>(ptr);
                    if (preUpdatePtr.get() != nullptr)
                        mPreUpdateSystems.push_back(Pair<PreUpdateSystem>{preUpdatePtr->orderPreUpdate, preUpdatePtr});
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
                    std::sort(mPreUpdateSystems.begin(), mPreUpdateSystems.end(), detail::Cmpr<SPtr<PreUpdateSystem>>());
                    std::sort(mPostUpdateSystems.begin(), mPostUpdateSystems.end(), detail::Cmpr<SPtr<PostUpdateSystem>>());
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
                    for (auto& pair : mPreUpdateSystems)
                    {
                        pair.val->preUpdate(dt);
                    }
                    for (auto& pair : mUpdateSystems)
                    {
                        pair.val->update(dt);
                    }
                    for (auto& pair : mPostUpdateSystems)
                    {
                        pair.val->postUpdate(dt);
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
                std::vector<Pair<PostUpdateSystem>> mPostUpdateSystems{};
                std::vector<Pair<PreUpdateSystem>> mPreUpdateSystems{};
                std::vector<Pair<RenderSystem>> mRenderSystems{};
                std::vector<Pair<InputSystem>> mInputSystems{};
        };
    }
} // namespace stay

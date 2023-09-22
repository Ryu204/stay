#include <type_traits>

#include "manager.hpp"

namespace stay
{
    namespace ecs
    {       
        Registry& Manager::getRegistry()
        {
            return mRegistry;
        } 

        // Meant to be called only once, before any update
        void Manager::start()
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
        void Manager::update(float dt)
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

        void Manager::render(RTarget* target)
        {
            for (auto& pair : mRenderSystems)
            {
                pair.val->render(target);
            }
        }

        void Manager::input(const sf::Event& event)
        {
            for (auto& pair : mInputSystems)
            {
                pair.val->input(event);
            }
        }

        Node* Manager::create(Node* parent)
        {
            auto* res = parent->createEmptyChild();
            return res;
        }
    }
} // namespace stay

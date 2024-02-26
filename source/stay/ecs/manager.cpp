#include "stay/ecs/manager.hpp"
#include "stay/program/builtinSystems.hpp"

namespace stay
{
    namespace ecs
    {       
        Manager& manager()
        {
            static Manager manager;
            return manager;
        }

        Registry& Manager::getRegistryRef()
        {
            return *mRegistry;
        } 

        SPtr<Registry> Manager::getRegistry()
        {
            return mRegistry;
        }

        void Manager::reset(SystemContext context, const Serializable::Data& systemConfigs)
        {
            stay::detail::registerBuiltinSystems();
            std::make_shared<Registry>().swap(mRegistry);
            // In the same category, system with smallest id gets called first and so on
            std::sort(mInitSystems.begin(), mInitSystems.end(), detail::Cmpr<InitSystem>());
            std::sort(mStartSystems.begin(), mStartSystems.end(), detail::Cmpr<StartSystem>());
            std::sort(mUpdateSystems.begin(), mUpdateSystems.end(), detail::Cmpr<UpdateSystem>());
            std::sort(mPreUpdateSystems.begin(), mPreUpdateSystems.end(), detail::Cmpr<PreUpdateSystem>());
            std::sort(mPostUpdateSystems.begin(), mPostUpdateSystems.end(), detail::Cmpr<PostUpdateSystem>());
            std::sort(mRenderSystems.begin(), mRenderSystems.end(), detail::Cmpr<RenderSystem>());
            std::sort(mInputSystems.begin(), mInputSystems.end(), detail::Cmpr<InputSystem>());
            std::sort(mConfigurableSystems.begin(), mConfigurableSystems.end(), detail::Cmpr<ConfigurableSystem>());

            // Configure and initialize
            for (auto& pair : mInitSystems)
            {
                pair.val->init(context);
            }
            for (auto& pair : mConfigurableSystems)
            {
                const auto& name = pair.name;
                if (!systemConfigs.contains(name))
                    throw std::runtime_error{"expected system name"};
                if (!pair.val->loadConfig(systemConfigs.at(name)))
                    throw std::runtime_error{"error configuring system"};
            }
        }

        // Meant to be called only once, before any update
        void Manager::start()
        {
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

        void Manager::render(RTarget* target, Node* root)
        {
            for (auto& pair : mRenderSystems)
            {
                pair.val->render(target, root);
            }
        }

        void Manager::input(const sf::Event& event)
        {
            for (auto& pair : mInputSystems)
            {
                pair.val->input(event);
            }
        }
    } // namespace ecs
} // namespace stay

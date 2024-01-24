#include "system.hpp"
#include "manager.hpp"

namespace stay
{
    namespace ecs
    {
        System::System(Manager* manager)
            : mManager(manager)
        {}
        Registry& System::registry() 
        {
            return mManager->getRegistryRef();
        }
        InitSystem::InitSystem(int order)
            : orderInit(order)
        {}
        StartSystem::StartSystem(int order)
            : orderStart(order)
        {}
        UpdateSystem::UpdateSystem(int order)
            : orderUpdate(order)
        {}
        PostUpdateSystem::PostUpdateSystem(int order)
            : orderLateUpdate(order)
        {}
        PreUpdateSystem::PreUpdateSystem(int order)
            : orderPreUpdate(order)
        {}
        RenderSystem::RenderSystem(int order)
            : orderRender(order)
        {}
        InputSystem::InputSystem(int order)
            : orderInput(order)
        {}
    } // namespace ecs
} // namespace stay

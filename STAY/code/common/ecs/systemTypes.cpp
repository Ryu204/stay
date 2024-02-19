#include "systemTypes.hpp"

namespace stay
{
    namespace ecs
    {
        InitSystem::InitSystem(int order)
            : orderInit{order}
        {}
        StartSystem::StartSystem(int order)
            : orderStart{order}
        {}
        UpdateSystem::UpdateSystem(int order)
            : orderUpdate{order}
        {}
        PostUpdateSystem::PostUpdateSystem(int order)
            : orderLateUpdate{order}
        {}
        PreUpdateSystem::PreUpdateSystem(int order)
            : orderPreUpdate{order}
        {}
        RenderSystem::RenderSystem(int order)
            : orderRender{order}
        {}
        InputSystem::InputSystem(int order)
            : orderInput{order}
        {}
        ConfigurableSystem::ConfigurableSystem(int order)
            : orderConfig{order}
        {}
    } // namespace ecs
} // namespace stay

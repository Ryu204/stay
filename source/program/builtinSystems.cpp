#include "stay/program/builtinSystems.hpp"

#include "stay/graphics/renderSystem.hpp"
#include "stay/physics/physicsDebugSystem.hpp"
#include "stay/physics/physicsSystem.hpp"

namespace stay 
{
    namespace detail
    {
        void registerBuiltinSystems()
        {
            ecs::manager().registerSystem<RenderSystem>("RenderSystem");
            ecs::manager().registerSystem<PhysicsDebugSystem>("PhysicsDebugSystem");
            ecs::manager().registerSystem<PhysicsSystem>("PhysicsSystem");
        }
    } // namespace detail
} // namespace stay
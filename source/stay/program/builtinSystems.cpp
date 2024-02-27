#include "stay/program/builtinSystems.hpp"

#include "stay/graphics/renderSystem.hpp"
#include "stay/physics/physicsDebugSystem.hpp"
#include "stay/physics/physicsSystem.hpp"

namespace stay 
{
    namespace detail
    {
        void registerBuiltinSystems(Builtin systems)
        {
            const auto flags = static_cast<int>(systems);
            if (static_cast<bool>(flags & static_cast<int>(Builtin::Render)))
                ecs::manager().registerSystem<RenderSystem>("RenderSystem");
            if (static_cast<bool>(flags & static_cast<int>(Builtin::PhysicsDebug)))
                ecs::manager().registerSystem<PhysicsDebugSystem>("PhysicsDebugSystem");
            if (static_cast<bool>(flags & static_cast<int>(Builtin::Physics)))
                ecs::manager().registerSystem<PhysicsSystem>("PhysicsSystem");
        }
    } // namespace detail
} // namespace stay
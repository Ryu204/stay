#include "stay/program/builtinSystems.hpp"

#include "stay/graphics/renderSystem.hpp"
#include "stay/physics/physicsDebugSystem.hpp"
#include "stay/physics/physicsSystem.hpp"

namespace stay 
{
    namespace detail
    {
        void registerBuiltinSystems(int systems)
        {
            if ((systems & systems::Render) != 0)
                ecs::manager().registerSystem<RenderSystem>("RenderSystem");
            if ((systems & systems::PhysicsDebug) != 0)
                ecs::manager().registerSystem<PhysicsDebugSystem>("PhysicsDebugSystem");
            if ((systems & systems::Physics) != 0)
                ecs::manager().registerSystem<PhysicsSystem>("PhysicsSystem");
        }
    } // namespace detail
} // namespace stay
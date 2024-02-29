#pragma once

namespace stay 
{
    namespace systems
    {
        enum Type
        {
            Render          = 1,
            Physics         = 2,    
            PhysicsDebug    = 4,
            
            All = Render | Physics | PhysicsDebug
        };
    } // namespace systems
    namespace detail
    {
        void registerBuiltinSystems(int systems = systems::Type::All);
    } // namespace detail
} // namespace stay
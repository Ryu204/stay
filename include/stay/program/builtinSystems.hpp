#pragma once

namespace stay 
{
    enum class Builtin
    {
        Render          = 1 << 0,
        Physics         = 1 << 1,
        PhysicsDebug    = 1 << 2,

        All = Render | Physics | PhysicsDebug
    };
    namespace detail
    {
        void registerBuiltinSystems(Builtin systems);
    } // namespace detail
} // namespace stay
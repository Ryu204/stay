#include "world.hpp"


namespace stay
{
    namespace phys
    {
        namespace details
        {           
            Uptr<b2World> WorldTrait::create()
            {
                return std::make_unique<b2World>(b2Vec2{0.F, -20.F});
            }
            void WorldTrait::shutdown(Uptr<b2World>& world)
            {
                world.reset();
            }
        } // namespace details
    } // namespace phys
} // namespace stay

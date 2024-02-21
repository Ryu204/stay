#include "stay/ecs/system.hpp"
#include "stay/physics/debugDraw.hpp"
#include "stay/physics/world.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    namespace sys
    {
        struct PhysicsDebugSystem 
            : public ecs::RenderSystem
            , public ecs::InitSystem
            , public ecs::InputSystem
            , public ecs::System
        {
                REGISTER_SYSTEM(PhysicsDebugSystem)
                PhysicsDebugSystem(ecs::Manager* manager)
                    : ecs::RenderSystem{1}
                    , ecs::InitSystem{0}
                    , ecs::InputSystem{0}
                    , ecs::System{manager}
                { }
                ~PhysicsDebugSystem() override
                {
                    // Clear the drawer
                    if (phys::World::avail())
                    {
                        phys::World::get().SetDebugDraw(nullptr);
                    }
                }

                void init(ecs::SystemContext& /* context */) override
                {
                    mShouldDraw = false;
                    mDrawer = std::make_unique<phys::DebugDraw>();
                    uint32 flags = 0;
                    flags += b2Draw::e_shapeBit;
                    // flags += b2Draw::e_jointBit;
                    // flags += b2Draw::e_aabbBit;
                    flags += b2Draw::e_centerOfMassBit;
                    mDrawer->SetFlags(flags);
                    phys::World::get().SetDebugDraw(mDrawer.get());
                }

                void input(const sf::Event& event) override
                {
                    if (event.type == sf::Event::KeyPressed
                        && event.key.code == sf::Keyboard::P
                        && event.key.control)
                    {
                        mShouldDraw = !mShouldDraw;
                    }
                }

                void render(RTarget* target, Node* /*root*/) override
                {
                    if (!mShouldDraw)
                        return;
                    mDrawer->setRenderTarget(target);
                    phys::World::get().DebugDraw();
                }

            private:
                Uptr<phys::DebugDraw> mDrawer;
                bool mShouldDraw;
        };
    } // namespace sys
} // namespace stay

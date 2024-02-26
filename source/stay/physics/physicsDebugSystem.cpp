#include "stay/physics/physicsDebugSystem.hpp"
#include "stay/physics/world.hpp"

namespace stay
{
    PhysicsDebugSystem::PhysicsDebugSystem(ecs::Manager* manager)
        : ecs::RenderSystem{1}
        , ecs::InitSystem{0}
        , ecs::InputSystem{0}
        , ecs::System{manager}
    { }
    PhysicsDebugSystem::~PhysicsDebugSystem()
    {
        // Clear the drawer
        if (phys::World::avail())
        {
            phys::World::get().SetDebugDraw(nullptr);
        }
    }
    void PhysicsDebugSystem::init(ecs::SystemContext& /* context */)
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
    void PhysicsDebugSystem::input(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed
            && event.key.code == sf::Keyboard::P
            && event.key.control)
        {
            mShouldDraw = !mShouldDraw;
        }
    }
    void PhysicsDebugSystem::render(RTarget* target, Node* /*root*/)
    {
        if (!mShouldDraw)
            return;
        mDrawer->setRenderTarget(target);
        phys::World::get().DebugDraw();
    }
} // namespace stay

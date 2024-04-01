#include "stay/physics/physicsDebugSystem.hpp"
#include "stay/physics/world.hpp"

#include "stay/physics/debugShape.hpp"

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
        renderDebugShapes(target);
    }
    void PhysicsDebugSystem::renderDebugShapes(RTarget* target)
    {
        std::vector<sf::RectangleShape> rects{};
        sf::VertexArray lines{sf::PrimitiveType::Lines};
        for (const auto [e, shape] : mManager->getRegistryRef().view<DebugShape>().each())
        {
            for (const auto& line : shape.lines)
            {
                lines.append(sf::Vertex{
                    line.start.toVec2<sf::Vector2f>(),
                    line.color,
                });
                lines.append(sf::Vertex{
                    line.end.toVec2<sf::Vector2f>(),
                    line.color,
                });
            }
            for (const auto& rect : shape.rectangles)
            {
                const auto position = rect.bounds.min().toVec2<sf::Vector2f>();
                const sf::Vector2f size{rect.bounds.width(), rect.bounds.height()};
                auto& r = rects.emplace_back(size);
                r.setPosition(position);
                r.setFillColor(rect.color);
            }
        }
        target->draw(lines);
        for (const auto& rect : rects)
            target->draw(rect);
    }
} // namespace stay

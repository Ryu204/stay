#include "dashSystem.hpp"
#include <SFML/Window/Event.hpp>
#include "../component/dash.hpp"
#include "physics/rigidBody.hpp"
#include "utility/invoke.hpp"

namespace stay 
{
    DashSystem::DashSystem(ecs::Manager* manager)
        : ecs::System(manager)
        , ecs::UpdateSystem(0)
        , ecs::InputSystem(0)
    {}

    void DashSystem::input(const sf::Event& event)
    {
        if (event.type != sf::Event::KeyPressed || event.key.scancode != sf::Keyboard::Scan::C)
            return;
        auto view = registry().view<Dash, phys::RigidBody>();
        for (const auto [entity, dash, body] : view.each())
        {
            if (dash.activated || !dash.canDash)
                return;
            dash.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A);
            dash.canDash = false;
            dash.activated = true;
            const float currentGravityScale = body.gravityScale();
            body.setGravityScale(0.F);
            Invoke::after(dash.cooldown, [&dash = dash]{
                dash.canDash = true;
            });
            Invoke::after(dash.length / dash.velocity, [currentGravityScale, &dash = dash, &body = body]{
                dash.activated = false;
                body.setGravityScale(currentGravityScale);
            });
        }
    }

    void DashSystem::update(float /*dt*/)
    {
        auto view = registry().view<Dash, phys::RigidBody>();
        for (const auto [entity, dash, body] : view.each())
        {
            if (dash.activated)
            {
                const Vector2 velocity{dash.left ? -dash.velocity : dash.velocity, 0.F};
                body.setVelocity(velocity);
            }
        }
    }
} // namespace stay
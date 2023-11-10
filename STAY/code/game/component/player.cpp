#include <optional>
#include <cmath>

#include "player.hpp"
#include "../../common/physics/collider.hpp"

namespace stay
{
   
    PlayerSystem::PlayerSystem(ecs::Manager* manager)
        : ecs::System(manager)
        , ecs::StartSystem(0)
        , ecs::InputSystem(0)
        , ecs::UpdateSystem(0)
    {}

    void PlayerSystem::start()
    {
        auto view = mManager->getRegistryRef().view<Player>();
        for (auto entity : view)
        {
            auto& player = mManager->getComponent<Player>(entity);
            player.rgbody = &player.getNode()->getComponent<phys::RigidBody>();
            // player.rgbody->setFixedRotation(true);
            auto& collider = player.getNode()->getComponent<phys::Collider>();
            collider.OnCollisionEnter.addEventListener(
                [&player](phys::Collision& contact)
                {
                    if (contact.normal.y < 0.5F)
                    {
                        player.canJump = true;
                        player.onGround = true;
                    }
                }
            );
            collider.OnCollisionExit.addEventListener(
                [&player](phys::Collision& contact) 
                {
                    if (contact.normal.y < 0.5F)
                    {
                        player.onGround = false;
                    }
                }
            );
        }
    }

    void PlayerSystem::input(const sf::Event& event)
    {
        mEntered = false;
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.scancode)
            {
                case sf::Keyboard::Scan::Space:
                    mEntered = true;
                    break;
                default:
                    break;
            }
        }

        if (mEntered)
        {
            auto view = mManager->getRegistryRef().view<Player>();
            for (auto [entity, player] : view.each())
            {
                if (!player.canJump)
                    continue;
                auto vel = player.rgbody->getVelocity();
                auto grav = player.rgbody->gravity();
                vel.y = std::sqrt(std::abs(2 * player.jumpHeight * grav.y * player.rgbody->gravityScale()));
                player.rgbody->setVelocity(vel);
                player.canJump = false;
            }
        }
    }

    void PlayerSystem::update(float /*dt*/)
    {
        Vector2 dir{0.F, 0.F};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
            dir.x -= 1.F;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
            dir.x += 1.F;
        auto view = mManager->getRegistryRef().view<Player>();
        for (auto [entity, player] : view.each())
        {
            const auto vel = player.rgbody->getVelocity();
            auto force = utils::convertVec2<Vector2>(dir * player.moveStrength);
            const auto isLeft = vel.x < 5.F;
            const auto isRight = vel.x > -5.F;
            const bool mayIncreaseForce = (dir.x < 0.F && isRight) || (dir.x > 0.F && isLeft);
            if (mayIncreaseForce && !player.onRope)
            {
                force = force * (player.onGround ? player.oppositeScale : player.airScale);
            }
            player.rgbody->applyForce(force);
        }
    }
} // namespace stay
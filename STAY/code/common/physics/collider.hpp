#pragma once

#include <box2d/box2d.h>

#include "ecs/component.hpp"
#include "colliderInfo.hpp"
#include "collision.hpp"
#include "material.hpp"
#include "layer.hpp"

namespace stay
{
    namespace sys
    {
        class PhysicsSystem;
    } // namespace sys
    namespace phys
    {
        class RigidBody;
        // @note Remember to call `start` after initialization to connect to `RigidBody`
        class Collider : public ecs::Component
        {
            public:
                // @param entity The entity this collider attaches to
                Collider(const ColliderInfo& info = Box{}, const Material& mat = Material{});
                // @brief connect to the rigidbody
                // This method must be called to make the collider work
                void connect();
                ~Collider() override;
                void setMaterial(const Material& mat);
                void setTrigger(bool isTrigger);
                bool getTrigger() const;
                const std::string& layer() const;
                void setLayer(const std::string& layer);
                void setLayer(int id);
                float mass() const;
                // @return size of the AABB box bounding the shape
                Vector2 getLocalBound() const;

                event::Event<Collision& /*info*/> OnCollisionEnter;
                event::Event<Collision& /*info*/> OnCollisionExit;
                event::Event<Collision& /*info*/> OnTriggerEnter;
                event::Event<Collision& /*info*/> OnTriggerExit;
                event::Event<Collision& /*info*/, b2Contact& /*native info*/> OnCollisionDetection;

                SERIALIZE(mMaterial, mShapeInfo)
            private:
                void attachToRigidBody();

                Material mMaterial;
                ColliderInfo mShapeInfo;
                b2Fixture* mFixture;

                static Layer& mCollisionLayer();

                friend class sys::PhysicsSystem;

                void check() const;
                friend class DestructRegister;
        };

        // @brief Placeholder to provide a mean for an entity to hold more than 1 collider
        struct Collider01 : public Collider 
        {
            using Collider::Collider;
        };
        struct Collider02 : public Collider 
        {
            using Collider::Collider;
        };
    } // namespace phys
} // namespace stay
#pragma once

#include <box2d/box2d.h>

#include "stay/ecs/component.hpp"
#include "colliderInfo.hpp"
#include "collision.hpp"
#include "material.hpp"
#include "layer.hpp"

namespace stay
{
    class PhysicsSystem;
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

                COMPONENT(Collider, mMaterial, mShapeInfo)
            private:
                void attachToRigidBody();

                Material mMaterial;
                ColliderInfo mShapeInfo;
                b2Fixture* mFixture;

                friend class stay::PhysicsSystem;
                static Layer& collisionLayers();

                void check() const;
                friend class DestructRegister;
        };

        class Collider01 : public Collider
        {
            public:
                using Collider::Collider;
                REGISTER_COMPONENT(Collider01);
        };
    } // namespace phys
} // namespace stay
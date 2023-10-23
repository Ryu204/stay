#pragma once

#include <variant>
#include <optional>

#include <SFML/System/NonCopyable.hpp>
#include <box2d/box2d.h>

#include "colliderInfo.hpp"
#include "../type/vector.hpp"
#include "../utility/convert.hpp"
#include "../utility/typedef.hpp"
#include "../ecs/component.hpp"
#include "../event/event.hpp"
#include "layer.hpp"
#include "collision.hpp"

namespace stay
{
    namespace sys
    {
        class PhysicsSystem;
    } // namespace sys
    namespace phys
    {
        class Collider;
        class Rigidbody;
        class Material : public Serializable
        {
            public:
                Material(float density = 1.F, float friction = 0.F, float restituition = 0.F);
                void setDensity(float density);
                void setFriction(float friction);
                void setRestituition(float restituition);
                b2FixtureDef& getFixtureDef();
                const b2FixtureDef& getFixtureDef() const;
                int layerID() const;
                
                Json::Value toJSONObject() const override;
                bool fetch(const Json::Value& value) override;
            private:
                b2FixtureDef mDef;
                int mLayerID{0};
        };
        class RigidBody;
        // @note Remember to call `start` after initialization to connect to `RigidBody`
        class Collider : public ecs::Component
        {
            public:
                // @param entity The entity this collider attaches to
                Collider(const ColliderInfo& info = Box{}, const Material& mat = Material());
                void start();
                ~Collider() override;
                void setMaterial(const Material& mat);
                void setTrigger(bool isTrigger);
                bool getTrigger() const;
                const std::string& layer() const;
                void setLayer(const std::string& layer);
                void setLayer(int id);
                float mass() const;

                event::Event<Collision&> OnCollisionEnter;
                event::Event<Collision&> OnCollisionExit;
                event::Event<Collision&> OnTriggerEnter;
                event::Event<Collision&> OnTriggerExit;

                SERIALIZE(mMaterial, mShapeInfo)
            private:
                void attachToRigidBody(RigidBody& rgbody);

                Material mMaterial;
                ColliderInfo mShapeInfo;
                b2Fixture* mFixture;

                static Layer& mCollisionLayer();

                friend class sys::PhysicsSystem;

                void check() const;
                friend class DestructRegister;
        };
    } // namespace phys
} // namespace stay
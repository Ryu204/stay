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

namespace stay
{
    namespace phys
    {
        class Collider;
        class Material : public Serializable
        {
            public:
                Material(float density = 1.F, float friction = 0.2F, float restituition = 0.F);
                void setDensity(float density);
                void setFriction(float friction);
                void setRestituition(float restituition);
                b2FixtureDef getFixtureDef() const;
                
                Json::Value toJSONObject() const override;
                bool fetch(const Json::Value& value) override;
            private:
                b2FixtureDef mDef;
        };
        class RigidBody;
        class Collider : public ecs::Component
        {
            public:
                Collider(const ColliderInfo& info = Box{}, const Material& mat = Material());
                virtual ~Collider();
                void start();
                void setMaterial(const Material& mat);
                void setTrigger(bool isTrigger);
                bool getTrigger() const;

                event::Event<Collider&, b2Contact&> OnCollisionEnter;
                event::Event<Collider&, b2Contact&> OnCollisionExit;
                event::Event<Collider&, b2Contact&> OnTriggerEnter;
                event::Event<Collider&, b2Contact&> OnTriggerExit;

                SERIALIZE(mMaterial, mShapeInfo)
            private:
                void attachToRigidBody();

                Material mMaterial;
                ColliderInfo mShapeInfo;
                b2Fixture* mFixture;
        };
    } // namespace phys
} // namespace stay
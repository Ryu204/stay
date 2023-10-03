#include "collider.hpp"
#include "rigidBody.hpp"
#include "../utility/typedef.hpp"
#include "../world/node.hpp"

namespace stay
{
    namespace phys
    {
        namespace detail
        {
            // Variant visitor
            template <typename... funcs>
            struct Visitor : funcs...
            {
                using funcs::operator()...;
            };
            // deduction guide
            template <typename... funcs>
            Visitor(funcs...) -> Visitor<funcs...>;
        } // namespace detail

        Collider::Collider(const ColliderInfo& info, const Material& mat) // NOLINT
            : mMaterial(mat)
            , mShapeInfo(info)
            , mFixture(nullptr)
        { }     

        Collider::~Collider()
        {
            // Must enable this check since `RigidBody` could have been destroyed prior to this call
            if (mFixture != nullptr && getNode()->hasComponent<RigidBody>())
            {
                mFixture->GetBody()->DestroyFixture(mFixture);
            }
        }

        void Collider::start()
        {
            assert(getNode()->hasComponent<RigidBody>() && "Collider created without a rigidbody");
            attachToRigidBody();
        }

        void Collider::setMaterial(const Material& mat)
        {
            const auto& def = mat.getFixtureDef();
            mFixture->SetDensity(def.density);
            mFixture->GetBody()->ResetMassData();
            mFixture->SetFriction(def.friction);
            mFixture->SetRestitution(def.restitution);
        }   

        void Collider::setTrigger(bool isTrigger)
        {
            mFixture->SetSensor(isTrigger);
        }

        bool Collider::getTrigger() const
        {
            return mFixture->IsSensor();
        }

        void Collider::attachToRigidBody()
        {
            b2FixtureDef def = mMaterial.getFixtureDef();
            auto shape = mShapeInfo.createShape();
            def.shape = shape.get();
            mFixture = getNode()->getComponent<RigidBody>().attachFixture(def);
            mFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        }

        Material::Material(float density, float friction, float restituition)
        {
            mDef.density = density;
            mDef.friction = friction;
            mDef.restitution = restituition;
        }

        void Material::setDensity(float density)
        {
            assert(density > 0.1F && "must have positive density");
            mDef.density = density;
        }

        void Material::setFriction(float friction)
        {
            mDef.friction = friction;
        }

        void Material::setRestituition(float restituition)
        {
            mDef.restitution = restituition;
        }

        b2FixtureDef Material::getFixtureDef() const
        {
            return mDef;
        }

        Json::Value Material::toJSONObject() const
        {
            Json::Value res;
            res["density"] = mDef.density;
            res["friction"] = mDef.friction;
            res["restituition"] = mDef.restitution;
            return res;
        }

        bool Material::fetch(const Json::Value& value)
        {
            if (!(value["density"].isNumeric() && value["friction"].isNumeric() && value["restituition"].isNumeric()))
                return false;
            mDef.density = value["density"].asFloat();
            mDef.friction = value["friction"].asFloat();
            mDef.restitution = value["restituition"].asFloat();
            return true;
        }
    } // namespace phys
} // namespace stay
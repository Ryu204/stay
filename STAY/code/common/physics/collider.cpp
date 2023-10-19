#include "collider.hpp"
#include "rigidBody.hpp"
#include "../utility/typedef.hpp"
#include "../world/node.hpp"
#include "../utility/math.hpp"

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
        { 
        }     

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
            attachToRigidBody(getNode()->getComponent<RigidBody>());
        }

        void Collider::setMaterial(const Material& mat)
        {
            const auto& def = mat.getFixtureDef();
            mFixture->SetDensity(def.density);
            mFixture->GetBody()->ResetMassData();
            mFixture->SetFriction(def.friction);
            mFixture->SetRestitution(def.restitution);
            setLayer(mat.layerID());
        }   

        void Collider::setTrigger(bool isTrigger)
        {
            mFixture->SetSensor(isTrigger);
        }

        bool Collider::getTrigger() const
        {
            return mFixture->IsSensor();
        }

        const std::string& Collider::layer() const
        {
            const std::uint16_t mask = (mFixture == nullptr) ? 
                mMaterial.getFixtureDef().filter.categoryBits :
                mFixture->GetFilterData().categoryBits;
            assert(mask != 0 && "zero collision mask");
            return mCollisionLayer().name(utils::mostSignificantBit(mask));
        }

        void Collider::setLayer(const std::string& layer)
        {
            auto mask = mCollisionLayer().getLayerMask(layer);
            auto colMask = mCollisionLayer().getCollisionMask(layer);
            if (mFixture == nullptr)
            {
                mMaterial.getFixtureDef().filter.categoryBits = mask;
                mMaterial.getFixtureDef().filter.maskBits = colMask;
            }                
            else
            {
                auto filter = mFixture->GetFilterData();
                filter.categoryBits = mask;
                filter.maskBits = colMask;
                mFixture->SetFilterData(filter);
            }
        }

        void Collider::setLayer(int id)
        {
            setLayer(mCollisionLayer().name(id));
        }

        void Collider::attachToRigidBody(RigidBody& rgbody)
        {
            b2FixtureDef def = mMaterial.getFixtureDef();
            auto shape = mShapeInfo.createShape();
            def.shape = shape.get();
            mFixture = rgbody.attachFixture(def);
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

        const b2FixtureDef& Material::getFixtureDef() const
        {
            return mDef;
        }

        b2FixtureDef& Material::getFixtureDef() 
        {
            return mDef;
        }

        int Material::layerID() const
        {
            return mLayerID;
        }

        Json::Value Material::toJSONObject() const
        {
            Json::Value res;
            res["density"] = mDef.density;
            res["friction"] = mDef.friction;
            res["restituition"] = mDef.restitution;
            res["layerID"] = utils::mostSignificantBit(mDef.filter.categoryBits);
            return res;
        }

        bool Material::fetch(const Json::Value& value)
        {
            if (!(value["density"].isNumeric() && value["friction"].isNumeric() 
                && value["restituition"].isNumeric() && value["layerID"].isNumeric()))
                return false;
            mDef.density = value["density"].asFloat();
            mDef.friction = value["friction"].asFloat();
            mDef.restitution = value["restituition"].asFloat();
            mLayerID = value["layerID"].asInt();
            return true;
        }
    } // namespace phys 
} // namespace stay
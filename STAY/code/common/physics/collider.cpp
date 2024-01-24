#include "collider.hpp"
#include "rigidBody.hpp"
#include "utility/math.hpp"
#include "utility/convert.hpp"

namespace stay
{
    namespace phys
    {
        Collider::Collider(const ColliderInfo& info, const Material& mat) // NOLINT
            : mMaterial(mat)
            , mShapeInfo(info)
            , mFixture(nullptr)
        { }     

        Collider::~Collider()
        {
            // Must enable this check since `RigidBody` could have been destroyed prior to this call
            if (mFixture != nullptr)
            {
                mFixture->GetBody()->DestroyFixture(mFixture);
            }
        }

        void Collider::connect()
        {
            assert(getNode()->hasComponent<RigidBody>() && "Collider created without a rigidbody");
            attachToRigidBody();
        }

        void Collider::setMaterial(const Material& mat)
        {
            check();
            mFixture->SetDensity(mat.density);
            mFixture->GetBody()->ResetMassData();
            mFixture->SetFriction(mat.friction);
            mFixture->SetRestitution(mat.restitution);
        }   

        void Collider::setTrigger(bool isTrigger)
        {
            check();
            mFixture->SetSensor(isTrigger);
        }

        bool Collider::getTrigger() const
        {
            check();
            return mFixture->IsSensor();
        }

        float Collider::mass() const
        {
            check();
            b2MassData data;
            mFixture->GetMassData(&data);
            return data.mass;
        }

        const std::string& Collider::layer() const
        {
            check();
            const std::uint16_t mask = (mFixture == nullptr) ? 
                mMaterial.filter.categoryBits :
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
                mMaterial.filter.categoryBits = mask;
                mMaterial.filter.maskBits = colMask;
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

        Vector2 Collider::getLocalBound() const
        {
            check();
            const auto& b = mFixture->GetAABB(0);
            return utils::convertVec2<Vector2>(b.upperBound - b.lowerBound);
        }

        void Collider::attachToRigidBody()
        {
            auto& rgbody = getNode()->getComponent<RigidBody>();
            auto shape = mShapeInfo.createShape();
            mMaterial.shape = shape.get();
            mFixture = rgbody.attachFixture(mMaterial);
            mFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        }

        void Collider::check() const
        {
            if (mFixture == nullptr)
                throw std::runtime_error("attempt to access a collider with null fixture");
        }

        Layer& Collider::mCollisionLayer() 
        {   
            static Layer res; 
            return res;
        }
    } // namespace phys 
} // namespace stay
#include "stay/physics/rigidBody.hpp"
#include "stay/physics/world.hpp"

namespace stay
{
    namespace phys
    {
        namespace detail
        {
            BodyDef::BodyDef(const b2BodyDef& other)
                : b2BodyDef{other}
            {}
                
            void BodyDef::postDeserialization() {
                type = static_cast<b2BodyType>(mBodyType);
                position = mPosition.toVec2<b2Vec2>();
            }
            void BodyDef::preSerialization() const {
                mPosition = Vector2::from(position);
                mBodyType = static_cast<int>(type);
            }

            void BodyDef::fromBody(b2Body& body) {
                type = body.GetType();
                position = body.GetPosition();
                angle = body.GetAngle() * RAD2DEG;
                gravityScale = body.GetGravityScale();
                linearDamping = body.GetLinearDamping();
                fixedRotation = body.IsFixedRotation();
            }
        } // namespace detail

        RigidBody::RigidBody(const Vector2& position, float angle, BodyType type)
            : mWorld(nullptr)
            , mBody(nullptr)
            , mHorizontalDamping(0.05F)
        {
            b2BodyDef bodyDef;
            bodyDef.position = position.toVec2<b2Vec2>();
            bodyDef.angle = angle * DEG2RAD;
            bodyDef.type = static_cast<b2BodyType>(type);
            bodyDef.angularDamping = 0.01F;
            
            mWorld = &phys::World::get();
            mBody = mWorld->CreateBody(&bodyDef);
            mBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        }

        BodyType RigidBody::type() const
        {
            return static_cast<BodyType>(mBody->GetType());
        }

        void RigidBody::setType(const BodyType& type)
        {
            auto b2type = static_cast<b2BodyType>(type);
                mBody->SetType(b2type);
        }

        RigidBody::~RigidBody()
        {
            if (mBody != nullptr)
                mWorld->DestroyBody(mBody);
        }
        
        void RigidBody::setPosition(const Vector2& position)
        {
            mBody->SetTransform(position.toVec2<b2Vec2>(), mBody->GetAngle());
        }

        Vector2 RigidBody::getPosition() const
        {
            return Vector2::from(mBody->GetPosition());
        }

        void RigidBody::setAngle(float angle)
        {
            mBody->SetTransform(mBody->GetPosition(), angle * DEG2RAD);
        }

        float RigidBody::getAngle() const
        {
            return mBody->GetAngle() * RAD2DEG;
        }

        void RigidBody::applyForce(const Vector2& force)
        {
            mBody->ApplyForceToCenter(force.toVec2<b2Vec2>(), true);
        }

        void RigidBody::setVelocity(const Vector2& velocity) 
        {
            mBody->SetLinearVelocity(velocity.toVec2<b2Vec2>());
        }

        Vector2 RigidBody::getVelocity() const
        {
            return Vector2::from(mBody->GetLinearVelocity());
        }

        void RigidBody::setAngularVelocity(float velocity)
        {
            mBody->SetAngularVelocity(velocity * DEG2RAD);
        }

        float RigidBody::getAngularVelocity() const
        {
            return mBody->GetAngularVelocity() * RAD2DEG;
        }

        void RigidBody::setFixedRotation(bool fixed)
        {
            mBody->SetFixedRotation(fixed);
        }

        void RigidBody::setGravityScale(float scale)
        {
            mBody->SetGravityScale(scale);
        }

        Vector2 RigidBody::gravity() const
        {
            return Vector2::from(mWorld->GetGravity());
        }

        float RigidBody::gravityScale() const
        {
            return mBody->GetGravityScale();
        }

        bool RigidBody::bullet() const
        {
            return mBody->IsBullet();
        }

        void RigidBody::setBullet(bool isBullet)
        {
            mBody->SetBullet(isBullet);
        }

        float RigidBody::linearDamping() const
        {
            return mBody->GetLinearDamping();
        }

        float RigidBody::horizontalDamping() const
        {
            return mHorizontalDamping;
        }

        void RigidBody::setHorizontalDamping(float damp)
        {
            mHorizontalDamping = damp;
        }

        void RigidBody::setLinearDamping(float damp)
        {
            mBody->SetLinearDamping(damp);
        }
        
        b2Fixture* RigidBody::attachFixture(const b2FixtureDef& properties)
        {
            return mBody->CreateFixture(&properties);
        }

        b2Body* RigidBody::body()
        {
            return mBody;
        }

        void RigidBody::preSerialization() const 
        {
            mBodyDefCache.fromBody(*mBody);
        }

        void RigidBody::postDeserialization() 
        {
            setType(static_cast<BodyType>(mBodyDefCache.type));
            setPosition(Vector2::from(mBodyDefCache.position));
            setAngle(mBodyDefCache.angle);
            setGravityScale(mBodyDefCache.gravityScale);
            setLinearDamping(mBodyDefCache.linearDamping);
            setFixedRotation(mBodyDefCache.fixedRotation);
        }
    } // namespace phys
} // namespace stay

#include "rigidBody.hpp"
#include "world.hpp"
#include "utility/convert.hpp"

namespace stay
{
    namespace phys
    {
        namespace detail
        {
            BodyDef::BodyDef(const b2BodyDef& other)
                : b2BodyDef{other}
            {}
                
            void BodyDef::postSerialization() {
                type = static_cast<b2BodyType>(mBodyType);
                position = utils::convertVec2<b2Vec2>(mPosition);
            }
            void BodyDef::preSerialization() const {
                mPosition = utils::convertVec2<Vector2>(position);
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
            bodyDef.position = utils::convertVec2<b2Vec2>(position);
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
            mBody->SetTransform(utils::convertVec2<b2Vec2>(position), mBody->GetAngle());
        }

        Vector2 RigidBody::getPosition() const
        {
            return utils::convertVec2<Vector2>(mBody->GetPosition());
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
            mBody->ApplyForceToCenter(utils::convertVec2<b2Vec2>(force), true);
        }

        void RigidBody::setVelocity(const Vector2& velocity) 
        {
            mBody->SetLinearVelocity(utils::convertVec2<b2Vec2>(velocity));
        }

        Vector2 RigidBody::getVelocity() const
        {
            return utils::convertVec2<Vector2>(mBody->GetLinearVelocity());
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
            return utils::convertVec2<Vector2>(mWorld->GetGravity());
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

        void RigidBody::postSerialization() 
        {
            setType(static_cast<BodyType>(mBodyDefCache.type));
            setPosition(utils::convertVec2<Vector2>(mBodyDefCache.position));
            setAngle(mBodyDefCache.angle);
            setGravityScale(mBodyDefCache.gravityScale);
            setLinearDamping(mBodyDefCache.linearDamping);
            setFixedRotation(mBodyDefCache.fixedRotation);
        }
    } // namespace phys
} // namespace stay

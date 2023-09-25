#include "rigidBody.hpp"

namespace stay
{
    namespace phys
    {
        RigidBody::RigidBody(const Vector2& position, float angle, BodyType type)
        {
            mBodyDef = b2BodyDef();
            mBodyDef->position = utils::convertVec2<b2Vec2>(position);
            mBodyDef->angle = angle * DEG2RAD;
            mBodyDef->type = static_cast<b2BodyType>(type);
            mBodyDef->linearDamping = 0.F;
            mBodyDef->angularDamping = 0.01F;
        }

        void RigidBody::start(b2World* world)
        {
            mWorld = world;
            mBody = mWorld->CreateBody(&(*mBodyDef));
            mBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
            mBodyDef = std::nullopt;
        }

        RigidBody::~RigidBody()
        {
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
        
        b2Fixture* RigidBody::attachFixture(const b2FixtureDef& properties)
        {
            return mBody->CreateFixture(&properties);
        }
    } // namespace phys
} // namespace stay

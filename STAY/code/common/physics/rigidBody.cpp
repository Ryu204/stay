#include "rigidBody.hpp"

#include <unordered_map>

namespace
{
    std::unordered_map<b2Body*, stay::phys::RigidBody*>& rigidBodyMap()
    {
        static std::unordered_map<b2Body*, stay::phys::RigidBody*> res;
        return res;
    }
} // namespace

namespace stay
{
    namespace phys
    {
        RigidBody* RigidBody::getRigidBody(b2Body* body)
        {
            assert(rigidBodyMap().find(body) != rigidBodyMap().end());
            return rigidBodyMap()[body];
        }

        RigidBody::RigidBody(b2World* world, const Vector2& position, float angle, BodyType type)
            : mWorld(world)
        {
            b2BodyDef def;
            def.position = utils::convertVec2<b2Vec2>(position);
            def.angle = angle * DEG2RAD;
            def.type = static_cast<b2BodyType>(type);
            def.linearDamping = 0.F;
            def.angularDamping = 0.01F;
            mBody = mWorld->CreateBody(&def);
            rigidBodyMap()[mBody] = this;
        }

        RigidBody::~RigidBody()
        {
            rigidBodyMap().erase(mBody);
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

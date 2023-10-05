#include "rigidBody.hpp"

namespace stay
{
    namespace phys
    {
        RigidBody::RigidBody(const Vector2& position, float angle, BodyType type)
            : mWorld(nullptr)
            , mBody(nullptr)
        {
            mBodyDef.position = utils::convertVec2<b2Vec2>(position);
            mBodyDef.angle = angle * DEG2RAD;
            mBodyDef.type = static_cast<b2BodyType>(type);
            mBodyDef.angularDamping = 0.01F;
        }

        void RigidBody::start(b2World* world)
        {
            mWorld = world;
            mBody = mWorld->CreateBody(&mBodyDef);
            mBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
        }

        BodyType RigidBody::type() const
        {
            bool started = mBody != nullptr;
            if (started)
                return static_cast<BodyType>(mBody->GetType());
            else 
                return static_cast<BodyType>(mBodyDef.type);
        }

        void RigidBody::setType(const BodyType& type)
        {
            bool started = mBody != nullptr;
            auto b2type = static_cast<b2BodyType>(type);
            if (started)
                mBody->SetType(b2type);
            else
                mBodyDef.type = b2type;
        }

        RigidBody::~RigidBody()
        {
            if (mWorld != nullptr)
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
            bool started = mBody != nullptr;
            if (!started)
                mBodyDef.fixedRotation = fixed;
            else
                mBody->SetFixedRotation(fixed);
        }

        void RigidBody::setGravityScale(float scale)
        {
            if (mBody != nullptr)
                mBody->SetGravityScale(scale);
            else
                mBodyDef.gravityScale = scale;
        }

        Vector2 RigidBody::gravity() const
        {
            assert(mWorld != nullptr && "RigidBody not started yet");
            return utils::convertVec2<Vector2>(mWorld->GetGravity());
        }

        float RigidBody::gravityScale() const
        {
            return (mBody == nullptr) ? mBodyDef.gravityScale : mBody->GetGravityScale();
        }
        
        b2Fixture* RigidBody::attachFixture(const b2FixtureDef& properties)
        {
            return mBody->CreateFixture(&properties);
        }

        Json::Value RigidBody::toJSONObject() const
        {
            Json::Value res;
            res["position"] = utils::convertVec2<Vector2>(mBodyDef.position).toJSONObject();
            res["angle"] = mBodyDef.angle;
            res["type"] = mBodyDef.type;
            return res;
        }
        bool RigidBody::fetch(const Json::Value& value)
        {
            Vector2 position;
            if (!(value["angle"].isNumeric() && value["type"].isInt() && position.fetch(value["position"])))
                return false;
            mBodyDef.angle = value["angle"].asFloat();
            mBodyDef.position = utils::convertVec2<b2Vec2>(position);
            mBodyDef.type = static_cast<b2BodyType>(value["type"].asInt());
            return true;
        }
    } // namespace phys
} // namespace stay

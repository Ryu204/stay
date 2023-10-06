#include "rigidBody.hpp"
#include "world.hpp"

namespace stay
{
    namespace phys
    {
        RigidBody::RigidBody(const Vector2& position, float angle, BodyType type)
            : mWorld(nullptr)
            , mBody(nullptr)
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
        
        b2Fixture* RigidBody::attachFixture(const b2FixtureDef& properties)
        {
            return mBody->CreateFixture(&properties);
        }

        Json::Value RigidBody::toJSONObject() const
        {
            Json::Value res;
            res["position"] = utils::convertVec2<Vector2>(getPosition()).toJSONObject();
            res["angle"] = getAngle();
            res["type"] = static_cast<int>(type());
            return res;
        }
        bool RigidBody::fetch(const Json::Value& value)
        {
            Vector2 position;
            if (!(value["angle"].isNumeric() && value["type"].isInt() && position.fetch(value["position"])))
                return false;
            setAngle(value["angle"].asFloat());
            setPosition(position);
            setType(static_cast<BodyType>(value["type"].asInt()));
            return true;
        }
    } // namespace phys
} // namespace stay

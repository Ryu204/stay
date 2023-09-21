#include "collider.hpp"
#include "rigidBody.hpp"
#include "../utility/typedef.hpp"

namespace stay
{
    namespace phys
    {
        namespace detail
        {
            template <typename... funcs>
            struct Visitor : funcs...
            {
                using funcs::operator()...;
            };
            // deduction guide
            template <typename... funcs>
            Visitor(funcs...) -> Visitor<funcs...>;
        } // namespace detail
       
        Collider::Collider(const Info& info, RigidBody* body, const Material* mat)
            : mFixture(nullptr)
        {
            attachToRigidBody(info, body, mat);
        }     

        void Collider::setMaterial(const Material* mat)
        {
            if (mat != nullptr)
            {
                mat->updateCollider(this);
            }
        }   

        void Collider::setTrigger(bool isTrigger)
        {
            mFixture->SetSensor(isTrigger);
        }

        bool Collider::getTrigger() const
        {
            return mFixture->IsSensor();
        }

        void Collider::attachToRigidBody(const Info& info, RigidBody* body, const Material* mat)
        {
            bool materialExists = (mat != nullptr);
            if (materialExists)
            {
                auto def = mat->getFixtureDef();
                def.shape = createShape(info);
                mFixture = body->attachFixture(def);
                delete def.shape;
            }
        }

        b2Shape* Collider::createShape(const Collider::Info& info)
        {
            return std::visit(detail::Visitor{
                [](const Collider::Circle& cir)
                {
                    auto* res = new b2CircleShape();
                    res->m_p.Set(cir.position.x, cir.position.y);
                    res->m_radius = cir.radius;
                    return static_cast<b2Shape*>(res);
                },
                [](const Collider::Box& box)
                {
                    auto* res = new b2PolygonShape();
                    res->SetAsBox(box.halfSize.x, box.halfSize.y, 
                        utils::convertVec2<b2Vec2>(box.position), 
                        box.angle * DEG2RAD);
                    return static_cast<b2Shape*>(res);
                }
            }, info);
        }
        
        Material::Material(float density, float friction, float restituition)
        {
            mDef.density = density;
            mDef.friction = friction;
            mDef.restitution = restituition;
        }

        void Material::setDensity(float density)
        {
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

        void Material::updateCollider(Collider* collider) const
        {
            auto* fixture = collider->mFixture;
            fixture->SetDensity(mDef.density);
            fixture->GetBody()->ResetMassData();
            fixture->SetFriction(mDef.friction);
            fixture->SetRestitution(mDef.restitution);
        }
    } // namespace phys
} // namespace stay
#include "collider.hpp"
#include "rigidBody.hpp"
#include "../utility/typedef.hpp"
#include "../world/node.hpp"

/*debug*/ #include <iostream>
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

        Collider::Collider(const Info& info, const Material& mat)
            : mMaterial(mat)
            , mShapeInfo(info)
            , mFixture(nullptr)
        {
            /*debug*/ std::cout << "creating collider" << std::endl;
        }     

        Collider::~Collider()
        {
            /*debug*/
            std::cout << "~collider" << std::endl;
            // Must enable this check since `RigidBody` could have been destroyed prior to this call
            if (getNode()->hasComponent<RigidBody>())
            {
                mFixture->GetBody()->DestroyFixture(mFixture);
            }
            /*debug*/
            std::cout << "~~collider" << std::endl;
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
            b2FixtureDef def = (*mMaterial).getFixtureDef();
            auto shape = createShape(*mShapeInfo);
            def.shape = shape.get();
            mFixture = getNode()->getComponent<RigidBody>().attachFixture(def);
            mFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
            // Cleanup
            mMaterial = std::nullopt;
            mShapeInfo = std::nullopt;
        }

        Uptr<b2Shape> Collider::createShape(const Collider::Info& info)
        {
            auto res = std::visit(detail::Visitor{
                [](const Collider::Circle& cir) -> Uptr<b2Shape>
                {
                    Uptr<b2CircleShape> res = std::make_unique<b2CircleShape>();
                    res->m_p.Set(cir.position.x, cir.position.y);
                    res->m_radius = cir.radius;
                    return std::move(res);
                },
                [](const Collider::Box& box) -> Uptr<b2Shape>
                {
                    Uptr<b2PolygonShape> res = std::make_unique<b2PolygonShape>();
                    res->SetAsBox(box.size.x / 2.F, box.size.y / 2.F, 
                        utils::convertVec2<b2Vec2>(box.position), 
                        box.angle * DEG2RAD);
                    return std::move(res);
                }
            }, info);
            return std::move(res);
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
    } // namespace phys
} // namespace stay
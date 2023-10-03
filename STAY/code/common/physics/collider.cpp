#include "collider.hpp"
#include "rigidBody.hpp"
#include "../utility/typedef.hpp"
#include "../world/node.hpp"

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

        Collider::Collider(const Info& info, const Material& mat) // NOLINT
            : mMaterial(mat)
            , mShapeInfo(info)
            , mFixture(nullptr)
        { }     

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
            b2FixtureDef def = mMaterial.getFixtureDef();
            auto shape = createShape(mShapeInfo);
            def.shape = shape.get();
            mFixture = getNode()->getComponent<RigidBody>().attachFixture(def);
            mFixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
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

        Json::Value Collider::Info::toJSONObject() const
        {
            Json::Value res;
            std::visit(detail::Visitor{
                [&res](const Collider::Circle& cir)
                {
                    res["type"] = "circle";
                    res["position"] = cir.position.toJSONObject();
                    res["radius"] = cir.radius;
                },
                [&res](const Collider::Box& box)
                {
                    res["type"] = "box";
                    res["position"] = box.position.toJSONObject();
                    res["angle"] = box.angle;
                    res["size"] = box.size.toJSONObject();
                }
            }, *this);
            return res;
        }

        bool Collider::Info::fetch(const Json::Value& value)
        {
            bool succeeded = true;
            if (!value["type"].isString())
                return false;
            if (value["type"].asString() == "circle")
                operator=(Circle{});
            else if (value["type"].asString() == "box")
                operator=(Box{});
            std::visit(detail::Visitor{
                [&](Collider::Circle& cir)
                {
                    if (!(cir.position.fetch(value["position"]) && value["radius"].isNumeric()))
                        succeeded = false;
                    cir.radius = value["radius"].asFloat();
                },
                [&](Collider::Box& box)
                {
                    if (!(box.position.fetch(value["position"]) && box.size.fetch(value["size"]) && value["angle"].isNumeric()))
                        succeeded = false;
                    box.angle = value["angle"].asFloat();
                }
            }, *this);
            return succeeded;
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

        Json::Value Material::toJSONObject() const
        {
            Json::Value res;
            res["density"] = mDef.density;
            res["friction"] = mDef.friction;
            res["restituition"] = mDef.restitution;
            return res;
        }

        bool Material::fetch(const Json::Value& value)
        {
            if (!(value["density"].isNumeric() && value["friction"].isNumeric() && value["restituition"].isNumeric()))
                return false;
            mDef.density = value["density"].asFloat();
            mDef.friction = value["friction"].asFloat();
            mDef.restitution = value["restituition"].asFloat();
            return true;
        }
    } // namespace phys
} // namespace stay
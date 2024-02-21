#include "stay/physics/jointInfo.hpp"
#include "stay/utility/variantHelper.hpp"
#include "stay/physics/rigidBody.hpp"
#include <box2d/b2_math.h>

namespace stay
{
    namespace phys
    {
        Prismatic::Prismatic(Vector2 anchorA, Vector2 anchorB, Vector2 axis)
            : anchorA(std::move(anchorA))
            , anchorB(std::move(anchorB))
            , axis(std::move(axis))
        {}

        Revolute::Revolute(Vector2 anchor)
            : anchor(std::move(anchor))
        {}

        JointInfo::JointInfo(ecs::Entity other, bool shouldCollide, JointData data)
            : connectStatus{shouldCollide, other}
            , data{std::move(data)}
        {}
        
        Serializable::Data JointInfo::serialize() const
        {
            Serializable::Data res;
            res["status"] = connectStatus.serialize();
            std::visit(utils::VariantVisitor{
                [&res](const Prismatic& pris) {
                    res["type"] = "prismatic";
                    pris.serialize().swap(res["data"]);
                },
                [&res](const Revolute& rev) {
                    res["type"] = "revolute";
                    rev.serialize().swap(res["data"]);
                }
            }, data);
            return res;
        }
            
        bool JointInfo::deserialization(const Serializable::Data& value)
        {
            if (!value.contains("status") || !connectStatus.deserialization(value["status"])) 
                return false;
            auto string = value["type"].is_string() ? value["type"].get<std::string>() : std::string();
            if (string == "prismatic")
                data = Prismatic{};
            else if (string == "revolute")
                data = Revolute{Vector2{}};
            else
                return false;
            return std::visit(utils::VariantVisitor{
                [&](Prismatic& obj) {
                    return obj.deserialization(value["data"]);
                },
                [&](Revolute& rev) {
                    return rev.deserialization(value["data"]);
                }
            }, data);
        }
        
        Uptr<b2JointDef> JointInfo::createDef(RigidBody& a, RigidBody& b) const
        {
            return std::visit(utils::VariantVisitor{
                [&](const Prismatic& pris) -> Uptr<b2JointDef>{
                    auto r1 = std::make_unique<b2PrismaticJointDef>();
                    r1->bodyA = a.body();
                    r1->bodyB = b.body();
                    r1->localAnchorA = a.body()->GetLocalPoint(pris.anchorA.toVec2<b2Vec2>());
                    r1->localAnchorB = b.body()->GetLocalPoint(pris.anchorB.toVec2<b2Vec2>());
                    r1->referenceAngle = r1->bodyB->GetAngle() - r1->bodyA->GetAngle();
                    r1->localAxisA = r1->bodyA->GetLocalVector(pris.axis.toVec2<b2Vec2>());
                    return std::move(r1);
                },
                [&](const Revolute& rev) -> Uptr<b2JointDef> {
                    auto r1 = std::make_unique<b2RevoluteJointDef>();
                    r1->Initialize(a.body(), b.body(), rev.anchor.toVec2<b2Vec2>());
                    return std::move(r1);
                }
            }, data);
        }
    } // namespace phys
} // namespace stay
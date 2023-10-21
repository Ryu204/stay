#include "jointInfo.hpp"
#include "../utility/variantHelper.hpp"
#include "../utility/convert.hpp"
#include "rigidBody.hpp"

#include <box2d/box2d.h>
#include <json/value.h>

namespace stay
{
    namespace phys
    {
        Prismatic::Prismatic(Vector2 anchor, Vector2 axis)
            : anchor(std::move(anchor))
            , axis(std::move(axis))
        {}
        
        Json::Value JointInfo::toJSONObject() const
        {
            Json::Value res;
            std::visit(utils::VariantVisitor{
                [&res](const Prismatic& pris) {
                    res["type"] = "prismatic";
                    pris.toJSONObject().swap(res["data"]);
                }
            }, *this);
            return res;
        }
            
        bool JointInfo::fetch(const Json::Value& value)
        {
            auto string = value["type"].isString() ? value["type"].asString() : std::string();
            if (string == "prismatic")
                operator=(Prismatic{});
            else
                return false;
            return std::visit(utils::VariantVisitor{
                [&](Prismatic& obj)
                {
                    return obj.fetch(value["data"]);
                }
            }, *this);
        }
        
        Uptr<b2JointDef> JointInfo::createDef(RigidBody& A, RigidBody& B) const
        {
            return std::visit(utils::VariantVisitor{
                [&](const Prismatic& pris) -> Uptr<b2JointDef>{
                    auto r1 = std::make_unique<b2PrismaticJointDef>();
                    r1->Initialize(
                        A.body(), 
                        B.body(), 
                        utils::convertVec2<b2Vec2>(pris.anchor),
                        utils::convertVec2<b2Vec2>(pris.axis)
                    );
                    return std::move(r1);
                }
            }, *this);
        }
    } // namespace phys
} // namespace stay
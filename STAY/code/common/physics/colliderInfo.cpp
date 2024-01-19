#include "colliderInfo.hpp"
#include "../utility/convert.hpp"
#include "../utility/variantHelper.hpp"

namespace stay
{
    namespace phys
    {
        Chain::Chain(std::vector<Vector2>& points)
            : mPoints(std::move(points))
        {}

        const std::vector<b2Vec2>& Chain::data() const
        {
            if (!mCached)
            {
                mCached = true;
                mData.clear();
                for (const auto& i : mPoints)
                    mData.emplace_back(utils::convertVec2<b2Vec2>(i));
            }
            return mData;
        }

        void Chain::postSerialization()
        {
            mCached = false;
        }

        Uptr<b2Shape> ColliderInfo::createShape() const
        {
            auto res = std::visit(utils::VariantVisitor{
                [](const Circle& cir) -> Uptr<b2Shape>
                {
                    Uptr<b2CircleShape> res = std::make_unique<b2CircleShape>();
                    res->m_p.Set(cir.position.x, cir.position.y);
                    res->m_radius = cir.radius;
                    return std::move(res);
                },
                [](const Box& box) -> Uptr<b2Shape>
                {
                    Uptr<b2PolygonShape> res = std::make_unique<b2PolygonShape>();
                    res->SetAsBox(box.size.x / 2.F, box.size.y / 2.F, 
                        utils::convertVec2<b2Vec2>(box.position), 
                        box.angle * DEG2RAD);
                    return std::move(res);
                },
                [](const Chain& chain) -> Uptr<b2Shape>
                {
                    Uptr<b2ChainShape> res = std::make_unique<b2ChainShape>();
                    res->CreateLoop(chain.data().data(), chain.data().size());
                    return std::move(res);
                }
            }, *this);
            return std::move(res);
        }

        Serializable::Data ColliderInfo::toJSONObject() const
        {
            Serializable::Data res;
            std::visit(utils::VariantVisitor{
                [&res](const Circle& cir)
                {
                    res["type"] = "circle";
                    res["data"] = cir.toJSONObject();
                },
                [&res](const Box& box)
                {
                    res["type"] = "box";
                    res["data"] = box.toJSONObject();
                },
                [&res](const Chain& chain)
                {
                    res["type"] = "chain";
                    res["data"] = chain.toJSONObject();
                }
            }, *this);
            return res;
        }

        bool ColliderInfo::fetch(const Serializable::Data& value)
        {
            if (!value["type"].is_string())
                return false;
            auto type = value["type"].get<std::string>();
            if (type == "circle")
                operator=(Circle{});
            else if (type == "box")
                operator=(Box{});
            else if (type == "chain")
                operator=(Chain{});
            else return false;
            return std::visit(utils::VariantVisitor{
                [&](Circle& obj)
                {
                    return obj.fetch(value["data"]);
                },
                [&](Box& obj)
                {
                    return obj.fetch(value["data"]);
                },
                [&](Chain& obj)
                {
                    return obj.fetch(value["data"]);
                }
            }, *this);
        }
    } // namespace phys
} // namespace stay
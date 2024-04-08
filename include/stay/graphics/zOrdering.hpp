#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "render.hpp"
#include "stay/utility/convert.hpp"

namespace stay
{
    namespace detail
    {
        struct ZOrderPack 
        {
            std::vector<sf::Vertex> vertices{};
            int order{};
            std::optional<std::string> textureId{};
            RenderPrimitive type{};

            ZOrderPack() = default;
            ZOrderPack(const ZOrderPack&) = delete;
            ZOrderPack(ZOrderPack&&) = default;
            ZOrderPack& operator = (ZOrderPack&&) = default;

            static ZOrderPack from(const sf::Transform& tf, const Render& renderInfo)
            {
                const auto hasTexture = renderInfo.textureInfo.has_value();
                const auto rectPosition = hasTexture 
                    ? Vector2{
                        renderInfo.size.x * (-renderInfo.textureInfo->pivot.x + 0.5F),
                        renderInfo.size.y * (renderInfo.textureInfo->pivot.y - 0.5F),
                    }
                    : Vector2{};
                ZOrderPack res{};
                res.vertices.resize(4);

                if (hasTexture)
                {
                    const auto& rect = renderInfo.textureInfo->rect;
                    res.vertices[0].texCoords = rect.min().toVec2<sf::Vector2f>();
                    res.vertices[1].texCoords = utils::convertVec2<sf::Vector2f>(rect.min() + Vector2{rect.width(), 0.F});
                    res.vertices[2].texCoords = utils::convertVec2<sf::Vector2f>(rect.min() + Vector2{0.F, rect.height()});
                    res.vertices[3].texCoords = rect.max().toVec2<sf::Vector2f>();
                }

                res.vertices[3].position = tf.transformPoint(utils::convertVec2<sf::Vector2f>(
                    rectPosition + Vector2{renderInfo.size.x / 2.F, -renderInfo.size.y / 2.F}));
                res.vertices[2].position = tf.transformPoint(utils::convertVec2<sf::Vector2f>(
                    rectPosition - renderInfo.size / 2.F));
                res.vertices[1].position = tf.transformPoint(utils::convertVec2<sf::Vector2f>(
                    rectPosition + renderInfo.size / 2.F));
                res.vertices[0].position = tf.transformPoint(utils::convertVec2<sf::Vector2f>(
                    rectPosition + Vector2{-renderInfo.size.x / 2.F, renderInfo.size.y / 2.F}));

                for (auto& v : res.vertices)
                    v.color = renderInfo.color;

                // a b
                // c d
                // [a, b, c, d] (triangle_strip) -> [a, b, c, b, c, d] (triangles)
                res.vertices.insert(res.vertices.cbegin() + 3, res.vertices[1]);
                res.vertices.insert(res.vertices.cbegin() + 4, res.vertices[2]);

                res.order = renderInfo.zOrder;
                res.textureId = hasTexture ? std::make_optional<std::string>(renderInfo.textureInfo->id) : std::optional<std::string>();
                res.type = RenderPrimitive::TRIANGLES;
                return std::move(res);
            }

            static ZOrderPack from(const sf::Transform& tf, const RenderArray& array)
            {
                ZOrderPack res{};
                res.vertices.reserve(array.vertices.size());

                for (const auto& v : array.vertices) 
                    res.vertices.emplace_back(
                        tf.transformPoint(v.position.toVec2<sf::Vector2f>()), 
                        v.color,
                        v.texCoords.toVec2<sf::Vector2f>()
                    );

                res.order = array.zOrder;
                res.textureId = array.textureId;
                res.type = array.type();
                return std::move(res);
            }

            bool operator < (const ZOrderPack& right) const
            {
                return order < right.order;
            }

            bool isSameDrawCall(const ZOrderPack& other) const
            {
                if (textureId.has_value() && other.textureId.has_value())
                {
                    if (textureId.value() != other.textureId.value())
                        return false;
                }
                else if (textureId.has_value() || other.textureId.has_value())
                    return false;
                return order == other.order && type == other.type;
            }
        };
    } // namespace detail
} // namespace stay
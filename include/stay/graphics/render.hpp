#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "textureInfo.hpp"
#include "stay/type/color.hpp"
#include "stay/ecs/component.hpp"

namespace stay
{
    enum class RenderPrimitive
    {
        TRIANGLE_STRIP = sf::PrimitiveType::TriangleStrip, 
        LINE_STRIP = sf::PrimitiveType::LineStrip,
    };
    struct Render : public ecs::Component
    {
        Render(Color color = Color(), Vector2 size = Vector2(), int zOrder = 0)
            : color{std::move(color)}
            , size{std::move(size)}
            , zOrder{zOrder}
        {}
        Render(Color color, Vector2 size, int zOrder, const std::string& textureId, const Rect& textureRect, const Vector2& pivot = Vector2{})
            : color{std::move(color)}
            , size{std::move(size)}
            , zOrder{zOrder}
            , textureInfo{std::make_optional<TextureInfo>(textureId, textureRect, pivot)}
        {}
        Render(Color color, Vector2 size, int zOrder, TextureInfo info)
            : color{std::move(color)}
            , size{std::move(size)}
            , zOrder{zOrder}
            , textureInfo{std::make_optional<TextureInfo>(std::move(info))}
        {}
        
        Color color;
        Vector2 size;
        int zOrder;
        std::optional<TextureInfo> textureInfo;
        COMPONENT(Render, color, size, textureInfo);
    };
    struct RenderVertex : Serializable
    {
        Vector2 texCoords;
        Vector2 position;
        Color color;
        RenderVertex(Vector2 texCoords = {}, Vector2 position = {}, Color color = Color{0xFFFFFFFF})
            : texCoords{std::move(texCoords)}
            , position{std::move(position)}
            , color{std::move(color)}
        {}
        SERIALIZE(texCoords, position, color);
    };
    struct RenderArray : ecs::Component 
    {
        RenderArray(std::string textureId, int zOrder = {}, RenderPrimitive type = {})
            : textureId{std::make_optional<std::string>(std::move(textureId))}
            , zOrder{zOrder}
        {
            setType(type);
        }
        RenderArray(int zOrder = {}, RenderPrimitive type = {})
            : zOrder{zOrder}
        {
            setType(type);
        }
        void setColor(const Color& color) 
        {
            for (auto & v : vertices) 
                v.color = color;
        }
        RenderPrimitive type() const 
        {
            return static_cast<RenderPrimitive>(mType);
        }
        void setType(RenderPrimitive t)
        {
            mType = static_cast<int>(t);
        }

        COMPONENT(RenderArray, vertices, textureId, zOrder, mType);

        std::vector<RenderVertex> vertices{};
        std::optional<std::string> textureId{};
        int zOrder;

        private:
            int mType{static_cast<int>(RenderPrimitive::TRIANGLE_STRIP)};
    };
} // namespace stay
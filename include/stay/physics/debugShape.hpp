#pragma once

#include "stay/ecs/component.hpp"
#include "stay/type/color.hpp"
#include "stay/type/rect.hpp"

namespace stay
{
    struct DebugShape : ecs::Component
    {
        struct Line : Serializable
        {
            Line(Vector2 start = {}, Vector2 end = {}, Color color = Color{0xFF0000FF})
                : start{std::move(start)}
                , end{std::move(end)}
                , color{std::move(color)}
            {}
            Vector2 start{};
            Vector2 end{};
            Color color{};
            SERIALIZE(start, end, color);
        };
        void addLine(const Vector2& start = {}, const Vector2& end = Vector2{4, 4}, const Color& color = Color{0xFF0000FF})
        {
            lines.emplace_back(start, end, color);
        }
        struct Rectangle : Serializable
        {
            Rectangle(Rect bounds = {}, Color color = Color{0x0000FFFF})
                : bounds{std::move(bounds)}
                , color{std::move(color)}
            {}
            Rect bounds{};
            Color color{};
            SERIALIZE(bounds, color);
        };
        void addRectangle(const Rect& rect, const Color& color = Color{0x0000FFFF})
        {
            rectangles.emplace_back(rect, color);
        }
        void clear()
        {
            lines.clear();
            rectangles.clear();
        }
        std::vector<Line> lines{};
        std::vector<Rectangle> rectangles{};

        COMPONENT(DebugShape, lines, rectangles);
    };
} // namespace stay
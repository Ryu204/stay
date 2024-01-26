#include "debugDraw.hpp"

#include "utility/convert.hpp"
#include "utility/sfutils.hpp"

namespace stay
{
    namespace phys
    {

        DebugDraw::DebugDraw(float stroke)
            : mCanvas(nullptr)
            , mStrokeWidth(stroke)
        {}
        void DebugDraw::setRenderTarget(sf::RenderTarget* target)
        {
            mCanvas = target;
        }
        // When drawing, all shapes are scaled around the origin
        void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
        {
            sf::ConvexShape polygon(vertexCount);
            sf::Vector2f center;
            for(int i = 0; i < vertexCount; i++)
            {
                auto vec = utils::convertVec2<sf::Vector2f>(vertices[i]);
                polygon.setPoint(i, vec); 
            }
            polygon.setOutlineThickness(-mStrokeWidth);
            polygon.setFillColor(sf::Color::Transparent);
            polygon.setOutlineColor(utils::sfColorFrom(color));

            mCanvas->draw(polygon);
        }
        void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
        {
            sf::ConvexShape polygon(vertexCount);
            sf::Vector2f center;
            for(int i = 0; i < vertexCount; i++)
            {
                auto vec = utils::convertVec2<sf::Vector2f>(vertices[i]);
                polygon.setPoint(i, vec);
            }
            polygon.setOutlineThickness(-mStrokeWidth);
            auto sfColor = utils::sfColorFrom(color);
            sfColor.a = (float)sfColor.a * 0.6F;
            polygon.setOutlineColor(utils::sfColorFrom(color));
            polygon.setFillColor(sfColor);

            mCanvas->draw(polygon);
        }

        void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
        {
            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(utils::convertVec2<sf::Vector2f>(center));
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(-mStrokeWidth);
            circle.setOutlineColor(utils::sfColorFrom(color));

            mCanvas->draw(circle);
        }

        void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
        {
            sf::CircleShape circle(radius);
            circle.setOrigin(radius, radius);
            circle.setPosition(utils::convertVec2<sf::Vector2f>(center));
            circle.setOutlineThickness(-mStrokeWidth);
            auto sfColor = utils::sfColorFrom(color);
            circle.setOutlineColor(utils::sfColorFrom(color));
            sfColor.a = (float)sfColor.a * 0.6F;
            circle.setFillColor(sfColor);

            b2Vec2 endPoint = center + radius * axis;
            sf::Vertex line[2] = 
            {
                sf::Vertex(utils::convertVec2<sf::Vector2f>(center), utils::sfColorFrom(color)),
                sf::Vertex(utils::convertVec2<sf::Vector2f>(endPoint), utils::sfColorFrom(color)),
            };

            mCanvas->draw(circle);
            mCanvas->draw(line, 2, sf::Lines);
        }
        void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(utils::convertVec2<sf::Vector2f>(p1), utils::sfColorFrom(color)),
                sf::Vertex(utils::convertVec2<sf::Vector2f>(p2), utils::sfColorFrom(color)),
            };

            mCanvas->draw(line, 2, sf::Lines);
        }
        void DebugDraw::DrawTransform(const b2Transform& xf)
        {
            static const float lineLength = 1.F;

            auto pos = utils::convertVec2<sf::Vector2f>(xf.p);
            /*b2Vec2 xAxis(b2Vec2(xf.p.x + (lineLength * xf.q.c), xf.p.y + (lineLength * xf.q.s)));*/
            auto xAxis = pos + utils::convertVec2<sf::Vector2f>(lineLength * xf.q.GetXAxis());
            sf::Vertex redLine[] = 
            {
                sf::Vertex(pos, sf::Color::Red),
                sf::Vertex(xAxis, sf::Color::Red)
            };

            // You might notice that the ordinate(Y axis) points downward unlike the one in Box2D testbed
            // That's because the ordinate in SFML coordinate system points downward while the OpenGL(testbed) points upward
            /*b2Vec2 yAxis(b2Vec2(xf.p.x + (lineLength * -xf.q.s), xf.p.y + (lineLength * xf.q.c)));*/
            auto yAxis = pos + utils::convertVec2<sf::Vector2f>(lineLength * xf.q.GetYAxis());
            sf::Vertex greenLine[] = 
            {
                sf::Vertex(pos, sf::Color::Green),
                sf::Vertex(yAxis, sf::Color::Green)
            };

            mCanvas->draw(redLine, 2, sf::Lines);
            mCanvas->draw(greenLine, 2, sf::Lines);
        }
        void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
        {
            DrawCircle(p, size, color);
        }
    } // namespace phys
} // namespace stay
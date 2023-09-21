#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include "../utility/convert.hpp"
#include "../utility/b2utils.hpp"
#include "../utility/sfutils.hpp"

namespace stay
{
    namespace phys
    {
        class DebugDraw : public b2Draw
        {
            public:
                DebugDraw(sf::RenderTarget* target, float meterToPixels = 32.F)
                    : mCanvas(target)
                    , mMeterToPixels(meterToPixels)
                {}
                void setRenderTarget(sf::RenderTarget* target)
                {
                    mCanvas = target;
                }
                // When drawing, all shapes are scaled around the origin
                void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
                {
                    sf::ConvexShape polygon(vertexCount);
                    sf::Vector2f center;
                    for(int i = 0; i < vertexCount; i++)
                    {
                        auto vec = utils::convertVec2<sf::Vector2f>(mMeterToPixels * vertices[i]);
                        // flooring the coords to fix distorted lines on flat surfaces
                        // they still show up though.. but less frequently
                        polygon.setPoint(i, sf::Vector2f(std::floor(vec.x), std::floor(vec.y))); 
                    }
                    polygon.setOutlineThickness(-1.f);
                    polygon.setFillColor(sf::Color::Transparent);
                    polygon.setOutlineColor(utils::sfColorFrom(color));

                    mCanvas->draw(polygon);
                }
                void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
                {
                    sf::ConvexShape polygon(vertexCount);
                    sf::Vector2f center;
                    for(int i = 0; i < vertexCount; i++)
                    {
                        auto vec = utils::convertVec2<sf::Vector2f>(vertices[i] * mMeterToPixels);
                        // flooring the coords to fix distorted lines on flat surfaces
                        // they still show up though.. but less frequently
                        polygon.setPoint(i, sf::Vector2f(std::floor(vec.x), std::floor(vec.y))); 
                    }
                    polygon.setOutlineThickness(-1.f);
                    auto sfColor = utils::sfColorFrom(color);
                    sfColor.a = (float)sfColor.a * 0.6F;
                    polygon.setOutlineColor(utils::sfColorFrom(color));
                    polygon.setFillColor(sfColor);

                    mCanvas->draw(polygon);
                }

                void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override
                {
                    sf::CircleShape circle(radius * mMeterToPixels);
                    circle.setOrigin(radius * mMeterToPixels, radius * mMeterToPixels);
                    circle.setPosition(utils::convertVec2<sf::Vector2f>(mMeterToPixels * center));
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineThickness(-1.f);
                    circle.setOutlineColor(utils::sfColorFrom(color));

                    mCanvas->draw(circle);
                }

                void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
                {
                    sf::CircleShape circle(radius * mMeterToPixels);
                    circle.setOrigin(radius * mMeterToPixels, radius * mMeterToPixels);
                    circle.setPosition(utils::convertVec2<sf::Vector2f>(mMeterToPixels * center));
                    circle.setOutlineThickness(-1.f);
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
                void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
                {
                    sf::Vertex line[] =
                    {
                        sf::Vertex(SFMLDebugDraw::B2VecToSFVec(p1), SFMLDebugDraw::GLColorToSFML(color)),
                        sf::Vertex(SFMLDebugDraw::B2VecToSFVec(p2), SFMLDebugDraw::GLColorToSFML(color))
                    };

                    m_window->draw(line, 2, sf::Lines);
                }
                void DrawTransform(const b2Transform& xf) override
                {
                    float lineLength = 0.4;

                    /*b2Vec2 xAxis(b2Vec2(xf.p.x + (lineLength * xf.q.c), xf.p.y + (lineLength * xf.q.s)));*/
                    b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
                    sf::Vertex redLine[] = 
                    {
                        sf::Vertex(SFMLDebugDraw::B2VecToSFVec(xf.p), sf::Color::Red),
                        sf::Vertex(SFMLDebugDraw::B2VecToSFVec(xAxis), sf::Color::Red)
                    };

                    // You might notice that the ordinate(Y axis) points downward unlike the one in Box2D testbed
                    // That's because the ordinate in SFML coordinate system points downward while the OpenGL(testbed) points upward
                    /*b2Vec2 yAxis(b2Vec2(xf.p.x + (lineLength * -xf.q.s), xf.p.y + (lineLength * xf.q.c)));*/
                    b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
                    sf::Vertex greenLine[] = 
                    {
                        sf::Vertex(SFMLDebugDraw::B2VecToSFVec(xf.p), sf::Color::Green),
                        sf::Vertex(SFMLDebugDraw::B2VecToSFVec(yAxis), sf::Color::Green)
                    };

                    m_window->draw(redLine, 2, sf::Lines);
                    m_window->draw(greenLine, 2, sf::Lines);
                }
            private:
                sf::RenderTarget* mCanvas;
                float mMeterToPixels;
        };
    } // namespace phys
} // namespace stay
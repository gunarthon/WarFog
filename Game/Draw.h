#ifndef DRAW_H
#define DRAW_H

#include "Utils.h"
#include "Challenge.h"
#include "Part.h"
#include "Camera.h"
#include "InterfaceManager.h"
#include "PartList.h"
#include <SFML/Graphics.hpp>

class Draw : public b2Draw
{
    public:
        explicit Draw(sf::RenderWindow &Window);
        virtual ~Draw();
        void Clear();
        void ChangeResolution(String newRes);
        void DrawWorld(Camera *camera, PartList *allParts, b2World *world, bool notStarted,
                       bool drawStatic = true, bool showJoints = true, bool showOutlines = true, Challenge *challenge = NULL);
        void DrawInterface(InterfaceManager *interface);



        void DrawJoint(b2Joint *joint);
        void DrawShape(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha, bool showOutlines = true, bool cannonball = false);
        void DrawCannon(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha, bool showOutlines = true);
        void DrawShapeForOutline(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha);
        void DrawCannonForOutline(b2Shape *shape,/* b2XForm *xf,*/ b2Color color, Num alpha);
        void DrawTempShape(int creatingItem, int actionStep, Num firstClickX, Num firstClickY, Num secondClickX, Num secondClickY, Num mouseX, Num mouseY);

        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
        {
            DrawSolidPolygon(vertices, vertexCount, color);
        }
        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
        {
            sf::ConvexShape convex;
            convex.setPointCount(vertexCount);

            // define the points
            for(int i = 0; i < vertexCount; i++)
                convex.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
            convex.setFillColor(sf::Color::Red);
            convex.setOutlineThickness(0);
            window->draw(convex);
        }
        void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
        {
            sf::CircleShape convex;
            convex.setOrigin(radius, radius);
            convex.setPosition(center.x, center.y);
            convex.setRadius(radius);
            convex.setFillColor(sf::Color::Red);
            convex.setOutlineThickness(0);
            window->draw(convex);
        }
        void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
        {
            sf::CircleShape convex;
            convex.setOrigin(radius, radius);
            convex.setPosition(center.x, center.y);
            convex.setRadius(radius);
            convex.setFillColor(sf::Color::Red);
            convex.setOutlineThickness(0);
            window->draw(convex);
        }
        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
        void DrawTransform(const b2Transform& xf) {}

    protected:

    private:
		b2Color s_jointColor;
		b2Color s_selectedColor;
		b2Color s_normalColor;
		b2Color s_uneditableColor;
		b2Color s_jointCreatingColor;
		b2Color s_staticColor;
		b2Color s_staticEditableColor;
		b2World *m_world;
		sf::RenderWindow *window;

		sf::Font font;
		sf::Texture background;
		sf::Texture selectedTexture;
		sf::Texture image;
		sf::Shader selectedShader;
		sf::Shader challengeShader;
		sf::Text message;
};

#endif // DRAW_H

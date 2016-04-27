#ifndef CIRCLE_H
#define CIRCLE_H

#include "ShapePart.h"
#include "Utils.h"


class Circle : public ShapePart
{
    public:
        Circle(sf::Vector2f pos, Num r, bool checkLimits = true);
        virtual ~Circle();
        Num GetArea();
        bool InsideShape(sf::Vector2f val, Num scale = 1, bool shapeOnly = false);
        ShapePart *MakeCopy();
        void Init(b2World *world, b2Body *body = NULL);
        bool IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH);
        void PrepareForResizing();
        bool equals(ShapePart *other);
        void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic = true,
                  bool showJoints = true, bool showOutlines = true,
                  sf::Texture *texture = NULL, sf::Shader *shader = NULL);

        Num radius;
		Num initRadius;
    protected:

    private:
};

#endif // CIRCLE_H

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ShapePart.h"


class Rectangle : public ShapePart
{
    public:
        Rectangle(Num nx, Num ny, Num nw, Num nh, bool checkLimits = true);
        virtual ~Rectangle();
        Num GetArea();
        void GetVertices(VectorList *verts);
        bool InsideShape(sf::Vector2f val, Num scale = 1, bool shapeOnly = false);
        void Move(sf::Vector2f val);
        ShapePart *MakeCopy();
        virtual void Init(b2World *world, b2Body *body = NULL);
        bool IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH);
        //VectorList *GetVerticesForOutline(Num thickness);
        void PrepareForResizing();
        bool equals(ShapePart *other);
        void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic = true,
                  bool showJoints = true, bool showOutlines = true,
                  sf::Texture *texture = NULL, sf::Shader *shader = NULL);
        //String ToString();

        Num x;
        Num y;
        Num w;
        Num h;

        Num initW;
        Num initH;

        bool isTank;

        static constexpr Num MIN_WIDTH = 0.1;
        static constexpr Num MAX_WIDTH = 10.0;

    protected:

    private:
};

#endif // RECTANGLE_H

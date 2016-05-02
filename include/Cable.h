#ifndef CABLE_H
#define CABLE_H

#include "JointPart.h"


class Cable : public JointPart
{
    public:
        Cable(ShapePart *p1, ShapePart *p2, sf::Vector2f pos, int index1, int index2, bool valid = true);
        virtual ~Cable();
        void Update(b2World *world, InputManager *input);
        bool InsideShape(sf::Vector2f val, Num scale, bool shapeOnly = false);
		void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic = true,
            bool showJoints = true, bool showOutlines = true,
            sf::Texture *texture = NULL, sf::Shader *shader = NULL);

    protected:
        Color color;
        int index1, index2;

    private:
};

#endif // CABLE_H

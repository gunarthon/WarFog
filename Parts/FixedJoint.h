#ifndef FIXEDJOINT_H
#define FIXEDJOINT_H

#include "JointPart.h"


class FixedJoint : public JointPart
{
    public:
        FixedJoint(ShapePart *p1, ShapePart *p2, sf::Vector2f pos, bool valid = true);
        virtual ~FixedJoint();
        Part *MakeCopy();
        void Update(b2World *world, InputManager *input);
        bool InsideShape(sf::Vector2f val, Num scale, bool shapeOnly = false);
		void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic = true,
            bool showJoints = true, bool showOutlines = true,
            sf::Texture *texture = NULL, sf::Shader *shader = NULL);

    protected:

    private:
};

#endif // FIXEDJOINT_H

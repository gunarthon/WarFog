#ifndef REVOLUTEJOINT_H
#define REVOLUTEJOINT_H

#include "JointPart.h"


class RevoluteJoint : public JointPart
{
    public:
        RevoluteJoint(ShapePart *p1, ShapePart *p2, sf::Vector2f pos, bool valid = true);
        virtual ~RevoluteJoint();
        void SetJointProperties(RevoluteJoint *other);
        Part *MakeCopy();
        void Init(b2World *world, b2Body *body = NULL);
        void CheckForBreakage(b2World *world);
        void Update(b2World *world, InputManager *input);
        bool InsideShape(sf::Vector2f val, Num scale, bool shapeOnly = false);
        void KeyInput(sf::Keyboard::Key key, bool up, bool replay);
        void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                  bool showJoints, bool showOutlines, sf::Texture *texture = NULL, sf::Shader *shader = NULL);
        //String ToString();

        bool enableMotor;
        sf::Keyboard::Key motorCWKey;
        sf::Keyboard::Key motorCCWKey;

        Num motorStrength;
        Num motorSpeed;
        Num motorLowerLimit;
        Num motorUpperLimit;
        bool hasLowerLimit;
        bool hasUpperLimit;
        bool isStiff;
        bool autoCW;
        bool autoCCW;

        bool isKeyDown1;
        bool isKeyDown2;
        bool wasKeyDown1;
        bool wasKeyDown2;
        Num targetJointAngle;
        Num prevJointAngle;


    protected:

    private:
};

#endif // REVOLUTEJOINT_H

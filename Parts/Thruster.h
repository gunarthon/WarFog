#ifndef THRUSTER_H
#define THRUSTER_H

#include "JointPart.h"

class ShapePart;
class Thruster;
typedef std::vector<Thruster*> ThrusterList;


class Thruster : public JointPart
{
    public:
        Thruster(ShapePart *p1, sf::Vector2f pos, bool valid = true);
        virtual ~Thruster();

        Part *MakeCopy();
        bool InsideShape(sf::Vector2f val, Num scale, bool shapeOnly = false);
        void Init(b2World *world, b2Body *body = NULL);
        void KeyInput(sf::Keyboard::Key key, bool up, bool replay);
        void Update(b2World *world, InputManager *input);
        void Move(sf::Vector2f val);
        void RotateAround(sf::Vector2f center, Num curAngle);
        //PartIdList *GetAttachedParts(PartIdList *partList = NULL);
        void PrepareForResizing();
        void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                  bool showJoints, bool showOutlines,
                  sf::Texture *texture = NULL, sf::Shader *shader = NULL);
        //String ToString();

        Num strength;
        Num angle;
        sf::Keyboard::Key thrustKey;
        bool autoOn;

        bool isBalloon;

    protected:

    private:
        bool isKeyDown;
        b2Vec2 relativeThrusterPos;

};

#endif // THRUSTER_H

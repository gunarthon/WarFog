#ifndef JOINTPART_H
#define JOINTPART_H

#include "Part.h"
#include "Utils.h"

class ShapePart;
class JointPart;

typedef std::vector<JointPart*> JointList;

class JointPart : public Part
{
    public:
        JointPart(ShapePart *p1, ShapePart *p2);
        virtual ~JointPart();
        b2Joint *GetJoint();
        ShapePart *GetOtherPart(ShapePart *p);
        void ClearParts();
        virtual void Init(b2World *world, b2Body *body = NULL);
        void UnInit(b2World *world);
        void Move(sf::Vector2f val);
        virtual void KeyInput(sf::Keyboard::Key key, bool up, bool replay);
        void RotateAround(sf::Vector2f center, Num curAngle);
        virtual void GetAttachedParts(Group *group, bool recursive = true);
        virtual void Disconnect();
        virtual void Connect(ShapePart *p1, ShapePart *p2 = NULL);
        virtual bool ShouldDeleteConnections();
        bool IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH);
        void PrepareForResizing();
		virtual String ToString();
		virtual void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                    bool showJoints, bool showOutlines,
                    sf::Texture *texture = NULL, sf::Shader *shader = NULL) = 0;

        Num part1Index;
        Num part2Index;
    protected:
        ShapePart *part1;
        ShapePart *part2;
        sf::Vector2f anchor;
        b2Joint *m_joint;


    private:
};

#endif // JOINTPART_H

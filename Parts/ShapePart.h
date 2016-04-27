#ifndef SHAPEPART_H
#define SHAPEPART_H

#include "Part.h"
#include "JointPart.h"
#include "Thruster.h"
#include "Color.h"

class ShapePart;
typedef std::vector<ShapePart*> ShapeList;

class ShapePart : public Part
{
    public:
        ShapePart(Num x, Num y);
        virtual ~ShapePart();
        b2Body *GetBody();
		b2Shape *GetShape();
		void AddJoint(JointPart *j);
		void AddThruster(Thruster *t);
		void RemoveJoint(JointPart *j);
		void RemoveThruster(Thruster *t);
		void Move(sf::Vector2f val);
		void RotateAround(sf::Vector2f center, Num curAngle);
		virtual ShapePart *MakeCopy() = 0;
		bool HeavierThan(ShapePart *other);
		Num GetMass();
		virtual Num GetArea() = 0;
		virtual void UnInit(b2World *world);
		bool IsShapePart();
		virtual void Update(b2World *world, InputManager *input);
		void KeyInput(sf::Keyboard::Key key, bool up, bool replay);
		void SetCollisionGroup(int grp);
		JointList *GetActiveJoints();
		ThrusterList *GetActiveThrusters();
		bool WillBeStatic(ShapeList *shapeList = NULL);
		virtual void GetAttachedParts(Group *group, bool recursive = true);
		virtual void Disconnect();
		virtual bool ShouldDeleteConnections();
		virtual void GetVertices(VectorList *verts);
		virtual bool equals(ShapePart *other);
		bool NumbersEqual(Num n1, Num n2);
		virtual void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                    bool showJoints, bool showOutlines, sf::Texture *texture, sf::Shader *shader) = 0;
		//virtual String ToString();

		void SetCollide(String value);
		String GetCollide();

		Color color;
        Num centerX;
		Num centerY;
		bool outline;
		bool collide;

    protected:
		Num density;
		Num angle;

		int m_collisionGroup;
		bool highlightForJoint;
		bool isBullet;
		bool terrain;
		bool undragable;

		b2Body *m_body;
		b2Fixture *m_fixture;
		b2Shape *m_shape;
		JointList m_joints;
		ThrusterList m_thrusters;

    private:
};

#endif // SHAPEPART_H

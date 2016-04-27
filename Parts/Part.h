#ifndef PART_H
#define PART_H

#include "Utils.h"
#include "Proprieties.h"
#include "Color.h"
#include "PartList.h"
#include "InputManager.h"
class Camera;

class Part;
struct
{
    Num minDensity = 1;
    Num maxDensity = 30;
    Num maxRJStrength = 30;
    Num maxRJSpeed = 30;
    Num maxSJStrength = 30;
    Num maxSJSpeed = 30;
    Num maxThrusterStrength = 30;

    Num minCircleRadius = 0.1;
    Num maxCircleRadius = 5.0;

    Color defaultColor = Color(98, 189, 155, 255);

}ControllerGameSettings;

class Part
{
    public:
        Part();
        virtual ~Part();
        bool IsEnabled();
        virtual void Init(b2World *world, b2Body *body = NULL);
        virtual void UnInit(b2World *world);
		virtual String ToString();
		virtual bool Load(StringList content);
        virtual bool InsideShape(sf::Vector2f val, Num scale = 1, bool shapeOnly = false) = 0;
		virtual Part *MakeCopy() = 0;
		virtual void Move(sf::Vector2f val) = 0;
        virtual void RotateAround(sf::Vector2f center, Num angle) = 0;
		virtual void KeyInput(sf::Keyboard::Key key, bool up, bool replay) = 0;
		virtual void Update(b2World *world, InputManager *input) = 0;
		virtual void GetAttachedParts(Group *group, bool recursive = true) = 0;
		virtual void Disconnect() = 0;
		virtual bool ShouldDeleteConnections() = 0;
		virtual bool IsShapePart();
		virtual bool IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH) = 0;
		virtual void PrepareForResizing() = 0;
		virtual void Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                    bool showJoints, bool showOutlines,
                    sf::Texture *texture = NULL, sf::Shader *shader = NULL) = 0;

        bool isEnabled;
        bool isStatic;
        bool isEditable;
        bool isInitted;
		bool isCameraFocus;
        bool checkedCollisionGroup;
        bool drawAnyway;
        bool isSandbox;
        bool valid;
        String type;

        Num rotateAngle;
        Num rotateOrientation;
        Num dragXOff;
        Num dragYOff;
        const unsigned id;

        Proprieties proprieties;
        PartList *allParts;

    protected:
        Utils utils;
        bool disconnected;


        static unsigned nextId;

    private:
};

#endif // PART_H

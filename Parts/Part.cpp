#include "Part.h"
#include "Camera.h"

unsigned Part::nextId = 0;
Part::Part() : id(nextId++)
{
    isEnabled = true;
    isStatic = false;
    isEditable = true;
    isInitted = false;
    isCameraFocus = false;
    checkedCollisionGroup = false;
    drawAnyway = true;
    isSandbox = false;
    disconnected = false;
    valid = true;
    allParts = NULL;

    proprieties.list.push_back(new Propriety("id", true, (int*) &id));
    proprieties.list.push_back(new Propriety("sandbox", BOOL, 1, false,  &isSandbox));
    proprieties.list.push_back(new Propriety("editable", BOOL, 1, false,  &isEditable));
}

Part::~Part()
{

}

bool Part::IsEnabled()
{
    return isEnabled;
}

void Part::Init(b2World *world, b2Body *body)
{
    isInitted = true;
}

void Part::UnInit(b2World *world)
{
    isInitted = false;
}

bool Part::IsShapePart()
{
    return false;
}

String Part::ToString()
{
    std::stringstream s;
    s << type << std::endl;
    s << proprieties.ToString();
    return s.str();
}

bool Part::Load(StringList content)
{
    return proprieties.Load(content);
}

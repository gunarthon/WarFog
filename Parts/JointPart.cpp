#include "JointPart.h"
#include "ShapePart.h"

JointPart::JointPart(ShapePart *p1, ShapePart *p2) : Part()
{
    part1Index = -1;
    part2Index = -1;
    m_joint = NULL;

    part1 = NULL;
    part2 = NULL;
    Connect(p1, p2);

    proprieties.list.push_back(new Propriety("part1Index", POINTER, -1, false, NULL, &part1Index));
    proprieties.list.push_back(new Propriety("part2Index", POINTER, -1, false, NULL, &part2Index));
    proprieties.list.push_back(new Propriety("anchorX", SLIDER, 1, false, NULL, &anchor.x, NULL));
    proprieties.list.push_back(new Propriety("anchorY", SLIDER, 1, false, NULL, &anchor.y, NULL));
}

JointPart::~JointPart()
{

}

void JointPart::ClearParts()
{
    part1 = NULL;
    part2 = NULL;
}

b2Joint *JointPart::GetJoint()
{
    return m_joint;
}

ShapePart *JointPart::GetOtherPart(ShapePart *p)
{
    if(p == part1)
        return part2;
    if(p == part2)
        return part1;
    return NULL;
}

bool JointPart::ShouldDeleteConnections()
{
    return false;
}

void JointPart::Init(b2World *world, b2Body *body)
{
    if(isInitted)
        return;
    if(part1)
        if(!part1->isInitted)
            part1->Init(world);

    if(part2)
        if(!part2->isInitted)
            part2->Init(world);
    Part::Init(world);
}

void JointPart::UnInit(b2World *world)
{
    if(!isInitted)
        return;
    Part::UnInit(world);
    m_joint = NULL;
}

void JointPart::Move(sf::Vector2f val)
{
    anchor += val;
}

void JointPart::RotateAround(sf::Vector2f center, Num curAngle)
{
    Move(-center);
    Num sin = std::sin(utils.DegToRad(curAngle));
    Num cos = std::cos(utils.DegToRad(curAngle));
    sf::Vector2f delta = sf::Vector2f(anchor.x * cos - anchor.y * sin - anchor.x,
                                       anchor.x * sin + anchor.y * cos - anchor.y);
    Move(delta + center);
}

void JointPart::GetAttachedParts(Group *group, bool recursive)
{
    if(std::find(group->begin(), group->end(), id) != group->end())
        return;

	group->push_back(id);

	if(part1)
    {
        if(recursive)
            part1->GetAttachedParts(group, recursive);
        else
            group->push_back(part1->id);
    }
	if(part2)
    {
        if(recursive)
            part2->GetAttachedParts(group, recursive);
        else
            group->push_back(part2->id);
    }
}
void JointPart::Connect(ShapePart *p1, ShapePart *p2)
{
    if(p1 && !part1)
    {
        part1 = p1;
        part1->AddJoint(this);
    }

    if(p2 && !part2)
    {
        part2 = p2;
        part2->AddJoint(this);
    }
}

void JointPart::Disconnect()
{
    if(disconnected)
        return;
    disconnected = true;

    if(part1)
    {
        /*GroupIt it = std::find(d->begin(), d->end(), part1->id);
        if(it == d->end())*/
            part1->RemoveJoint(this);
    }
    if(part2)
    {
        /*GroupIt it = std::find(d->begin(), d->end(), part2->id);
        if(it == d->end())*/
            part2->RemoveJoint(this);
    }
    part1 = NULL;
    part2 = NULL;
}

bool JointPart::IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH)
{
    return (anchor.x >= boxX && anchor.x <= boxX + boxW &&
            anchor.y >= boxY && anchor.y <= boxY + boxH);
}

void JointPart::PrepareForResizing()
{

}

String JointPart::ToString()
{
    if(!allParts)
        return "";

    part1Index = allParts->IndexOf(part1);
    part2Index = allParts->IndexOf(part2);
    if(part1 && (part1Index == -1))
        return "";
    if(part2 && (part2Index == -1))
        return "";


    return Part::ToString();
}

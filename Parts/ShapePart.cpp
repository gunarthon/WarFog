#include "ShapePart.h"

ShapePart::ShapePart(Num x, Num y) : Part()
{
    collide = true;
    isCameraFocus = false;
    m_collisionGroup = -1;
    highlightForJoint = false;
    isBullet = false;
    centerX = x;
    centerY = y;
    if(ControllerGameSettings.minDensity > 15.0) {
        density = ControllerGameSettings.minDensity;
    } else if (ControllerGameSettings.maxDensity < 15.0) {
        density = ControllerGameSettings.maxDensity;
    } else {
        density = 15.0;
    }
    angle = 0;
    color = ControllerGameSettings.defaultColor;
    outline = true;
    terrain = false;
    undragable = false;

    m_body = NULL;
    m_fixture = NULL;
    m_shape = NULL;

    proprieties.list.push_back(new Propriety("static", BOOL, 1, true, &isStatic));
    proprieties.list.push_back(new Propriety("collide", BOOL, 1, true, &collide));
    proprieties.list.push_back(new Propriety("cameraFocus", BOOL, 0, true, &isCameraFocus));
    proprieties.list.push_back(new Propriety("outline", BOOL, 1, true, &outline));
    //proprieties.list.push_back(new Propriety("terrain", BOOL, 0, &terrain));
    //proprieties.list.push_back(new Propriety("undragable", BOOL, 0, &undragable));
    proprieties.list.push_back(new Propriety("angle", SLIDER, 1, true, NULL, &angle, NULL, -360, 360));
    proprieties.list.push_back(new Propriety("color.r", SLIDER, 1, true, NULL, &color.r, NULL, 0, 255));
    proprieties.list.push_back(new Propriety("color.g", SLIDER, 1, true, NULL, &color.g, NULL, 0, 255));
    proprieties.list.push_back(new Propriety("color.b", SLIDER, 1, true, NULL, &color.b, NULL, 0, 255));
    proprieties.list.push_back(new Propriety("color.a", SLIDER, 1, true, NULL, &color.a, NULL, 0, 255));

    proprieties.list.push_back(new Propriety("density", SLIDER, 1, true, NULL, &density, NULL, 0, 30));

    proprieties.list.push_back(new Propriety("centerX", SLIDER, 1, false, NULL, &centerX, NULL));
    proprieties.list.push_back(new Propriety("centerY", SLIDER, 1, false, NULL, &centerY, NULL));
}

ShapePart::~ShapePart()
{
    for(unsigned i = 0; i < m_joints.size(); i++)
    {
        JointPart *temp = m_joints[i];
        m_joints[i] = NULL;
        temp->GetOtherPart(this)->RemoveJoint(temp);
        temp->ClearParts();
        delete temp;
    }
    for(unsigned i = 0; i < m_thrusters.size(); i++)
    {
        Thruster *temp = m_thrusters[i];
        m_thrusters[i] = NULL;
        delete temp;
    }
}
b2Body *ShapePart::GetBody()
{
    return m_body;
}

b2Shape *ShapePart::GetShape()
{
    return m_shape;
}

void ShapePart::AddJoint(JointPart *j)
{
    m_joints.push_back(j);
}

void ShapePart::AddThruster(Thruster *t)
{
    m_thrusters.push_back(t);
}

void ShapePart::RemoveJoint(JointPart *j)
{
    if(!disconnected)
        m_joints.erase(std::find(m_joints.begin(), m_joints.end(), j));
}

void ShapePart::RemoveThruster(Thruster *t)
{
    if(!disconnected)
        m_thrusters.erase(std::find(m_thrusters.begin(), m_thrusters.end(), t));
}

bool ShapePart::IsShapePart()
{
    return true;
}

void ShapePart::Disconnect()
{
    if(disconnected)
        return;
    disconnected = true;

    for(unsigned i = 0; i < m_joints.size(); i++)
        m_joints.at(i)->Disconnect();
    m_joints.clear();

    for(unsigned i = 0; i < m_thrusters.size(); i++)
        m_thrusters.at(i)->Disconnect();
    m_thrusters.clear();
}
bool ShapePart::ShouldDeleteConnections()
{
    return true;
}

void ShapePart::Move(sf::Vector2f val)
{
	centerX += val.x;
	centerY += val.y;
}

void ShapePart::RotateAround(sf::Vector2f center, Num curAngle)
{
    Num dist = utils.GetLength(sf::Vector2f(centerX, centerY) - center);
    angle += curAngle;
    angle = utils.NormalizeAngle(angle);
    Move(sf::Vector2f(dist * std::cos(utils.DegToRad(curAngle)), dist * std::sin(utils.DegToRad(curAngle))));
}

bool ShapePart::HeavierThan(ShapePart *other)
{
    return (m_body->GetMass() > other->GetBody()->GetMass());
}

Num ShapePart::GetMass()
{
    if(m_body)
        return m_body->GetMass();
    return GetArea() * (density + 5.0) / 10.0;
}

void ShapePart::UnInit(b2World *world)
{
	if(!isInitted)
        return;
	Part::UnInit(world);
	if(m_body)
	{
	    //TODO check if correct
		m_body = NULL;
	}

}

void ShapePart::Update(b2World *world, InputManager *input)
{
    // do nothing
}

void ShapePart::KeyInput(sf::Keyboard::Key key, bool up, bool replay)
{
    // do nothing
}

void ShapePart::SetCollisionGroup(int grp)
{
	if(!checkedCollisionGroup)
    {
		checkedCollisionGroup = true;
		m_collisionGroup = grp;
		for(unsigned i = 0; i < m_joints.size(); i++)
		{
			if(m_joints.at(i)->isEnabled)
				m_joints.at(i)->GetOtherPart(this)->SetCollisionGroup(grp);
		}
	}

}

JointList *ShapePart::GetActiveJoints()
{
    //TODO:    return m_joints.filter(IsEnabled);
    return &m_joints;
}

ThrusterList *ShapePart::GetActiveThrusters()
{
    //TODO return m_thrusters.filter(IsEnabled);
    return &m_thrusters;
}

bool ShapePart::WillBeStatic(ShapeList *shapeList)
{
	if(isStatic)
        return true;
    if(shapeList == NULL)
        shapeList = new ShapeList();
    shapeList->push_back(this);
    for(unsigned i = 0; i < m_joints.size(); i++)
    {
        if(m_joints.at(i)->type == "FixedJoint")
        {
            ShapePart *otherShape = m_joints.at(i)->GetOtherPart(this);
            if(std::find(shapeList->begin(), shapeList->end(), otherShape) == shapeList->end() && otherShape->WillBeStatic(shapeList))
                return true;
        }
    }
    return false;
}

void ShapePart::GetAttachedParts(Group *group, bool recursive)
{
    if(std::find(group->begin(), group->end(), id) != group->end())
        return;
    group->push_back(id);

	if(recursive)
    {
        for(unsigned i = 0; i < m_joints.size(); i++)
            m_joints[i]->GetAttachedParts(group, recursive);

        for(unsigned i = 0; i < m_thrusters.size(); i++)
            m_thrusters[i]->GetAttachedParts(group, recursive);

    }
    else
    {
        for(unsigned i = 0; i < m_joints.size(); i++)
            group->push_back(m_joints[i]->id);
        for(unsigned i = 0; i < m_thrusters.size(); i++)
            group->push_back(m_thrusters[i]->id);
    }
}

void ShapePart::GetVertices(VectorList *verts)
{
	//TODO
	if(m_body && m_fixture)
    {
        const b2Transform& xf = m_body->GetTransform();
        this->m_shape = m_fixture->GetShape();

        b2PolygonShape *pol = (b2PolygonShape*) m_shape;

        // define the points
        for(int i = 0; i < pol->GetVertexCount(); i++)
        {
            b2Vec2 point = b2Mul(xf, pol->GetVertex(i));
            verts->push_back(point);
        }
    }
	/*if(numVerts == 3) {
		b2Vec2 center = Util.Vector((verts[0].x + verts[1].x + verts[2].x) / 3, (verts[0].y + verts[1].y + verts[2].y) / 3);
		Num length1 = Util.GetDist(verts[0].x, verts[0].y, verts[1].x, verts[1].y);
		Num length2 = Util.GetDist(verts[0].x, verts[0].y, verts[2].x, verts[2].y);
		Num length3 = Util.GetDist(verts[1].x, verts[1].y, verts[2].x, verts[2].y);
		Num avgLength = (length1 + length2 + length3) / 3;
		Num a1 = Util.NormalizeAngle(Math.acos((length1 * length1 + length2 * length2 - length3 * length3) / (2 * length1 * length2)));
		Num a2 = Util.NormalizeAngle(Math.acos((length1 * length1 + length3 * length3 - length2 * length2) / (2 * length1 * length3)));
		Num a3 = Util.NormalizeAngle(Math.acos((length2 * length2 + length3 * length3 - length1 * length1) / (2 * length2 * length3)));
		Num minAngle = Math.min(a1, a2, a3) * 180 / Math.PI;
		Num angleFactor = (minAngle < 30 ? 30 / minAngle : 1)
		Num scaleFactor = 1 + 2.5 * thickness / avgLength * angleFactor;

		newVerts.push(Util.Vector(center.x + (verts[0].x - center.x) * scaleFactor, center.y + (verts[0].y - center.y) * scaleFactor));
		newVerts.push(Util.Vector(center.x + (verts[1].x - center.x) * scaleFactor, center.y + (verts[1].y - center.y) * scaleFactor));
		newVerts.push(Util.Vector(center.x + (verts[2].x - center.x) * scaleFactor, center.y + (verts[2].y - center.y) * scaleFactor));
	} else {
		for (var i:int = 0; i < numVerts; i++) {
			var prev:b2Vec2 = (i == 0 ? verts[numVerts - 1] : verts[i - 1]);
			var cur:b2Vec2 = verts[i];
			var next:b2Vec2 = (i == numVerts - 1 ? verts[0] : verts[i + 1]);
			var angle1:Number = Math.atan2(prev.y - cur.y, prev.x - cur.x);
			var angle2:Number = Math.atan2(next.y - cur.y, next.x - cur.x);
			var bisectorAngle:Number = Util.NormalizeAngle((angle1 + angle2) / 2 + (Math.max(angle1, angle2) - Math.min(angle1, angle2) < Math.PI ? Math.PI : 0));
			newVerts.push(Util.Vector(cur.x + Math.cos(bisectorAngle) * thickness * 1.1, cur.y + Math.sin(bisectorAngle) * thickness * 1.1));
		}
	}
	*/
}

bool ShapePart::equals(ShapePart *other)
{
    return (isStatic == other->isStatic && NumbersEqual(centerX, other->centerX) &&
            NumbersEqual(centerY, other->centerY) && NumbersEqual(density, other->density) &&
            NumbersEqual(angle, other->angle) && collide == other->collide &&
            color == other->color && outline == other->outline &&
            terrain == other->terrain && undragable == other->undragable);
}

bool ShapePart::NumbersEqual(Num n1, Num n2)
{
    return (std::abs(n1 - n2) < 0.0001);
}

/*String ShapePart::ToString()
{
    std::stringstream s;
    s << "centerX=" << centerX << ", " << "centerY=" << centerY << ", " << "density=" << density << ", " << "angle=" << angle << ", " << "collide=" << collide << ", " << Part::ToString();
    return s.str();
}*/

void ShapePart::SetCollide(String value)
{
    this->collide = value != "0";
}
String ShapePart::GetCollide()
{
    return utils.ToString(collide);
}

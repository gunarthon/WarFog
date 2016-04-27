#include "Rectangle.h"
#include "Camera.h"

Rectangle::Rectangle(Num nx, Num ny, Num nw, Num nh, bool checkLimits) : ShapePart(x, y)
{
    isTank = false;

    if(checkLimits)
    {
        if(nw < 0)
        {
            nx += -MAX_WIDTH > nw ? -MAX_WIDTH : nw;
            nw = -nw;
        }
        if(nh < 0)
        {
            ny += -MAX_WIDTH > nh ? -MAX_WIDTH : nh;
            nh = -nh;
        }
        if(nw < MIN_WIDTH) nw = MIN_WIDTH;
        if(nh < MIN_WIDTH) nh = MIN_WIDTH;
        if(nw > MAX_WIDTH) nw = MAX_WIDTH;
        if(nh > MAX_WIDTH) nh = MAX_WIDTH;
    }

    x = nx;
    y = ny;
    w = nw;
    h = nh;

    //TODO check
    centerX = x + w / 2;
    centerY = y + h / 2;

    type = "Rectangle";

    proprieties.list.push_back(new Propriety("x", SLIDER, 1,  false, NULL, &x, NULL));
    proprieties.list.push_back(new Propriety("y", SLIDER, 1,  false, NULL, &y, NULL));
    proprieties.list.push_back(new Propriety("width", SLIDER, 1,  false, NULL, &w, NULL));
    proprieties.list.push_back(new Propriety("height", SLIDER, 1,  false, NULL, &h, NULL));
}

Rectangle::~Rectangle()
{

}

Num Rectangle::GetArea()
{
    return w * h;
}

void Rectangle::GetVertices(VectorList *verts)
{
    verts->clear();
    if(m_body && m_fixture)
    {
        ShapePart::GetVertices(verts);
        return;
    }

    verts->push_back(b2Vec2(x, y));
    verts->push_back(b2Vec2(x + w, y));
    verts->push_back(b2Vec2(x + w, y + h));
    verts->push_back(b2Vec2(x, y + h));
    Num dist = std::sqrt(w * w / 4 + h * h / 4);

    for (int i = 0; i < 4; i++)
    {
        Num vertAngle = std::atan2((verts->at(i)).y - centerY, (verts->at(i)).x - centerX);
        vertAngle = utils.NormalizeAngle(utils.DegToRad(angle) + vertAngle, true);
        verts->at(i).x = centerX + dist * std::cos((vertAngle));
        verts->at(i).y = centerY + dist * std::sin((vertAngle));
    }
}

bool Rectangle::InsideShape(sf::Vector2f val, Num scale, bool shapeOnly)
{
    bool allOnRightSide = true;
    VectorList verts;
    GetVertices(&verts);
    for(int i = 0; i < 4; i++)
    {
        Num slope;
        if(verts.at((i + 1) % 4).x == verts.at(i).x)
        {
            if(verts.at((i + 1) % 4).y >= verts.at(i).y)
            {
                slope = 100000000;
            }
            else
                slope = -100000000;
        }
        else
            slope = (verts.at((i + 1) % 4).y - verts.at(i).y) / (verts.at((i + 1) % 4).x - verts.at(i).x);

        Num val1 = val.y - verts.at(i).y - slope * (val.x - verts.at(i).x);
        Num val2 = verts.at((i + 2) % 4).y - verts.at(i).y - slope * (verts.at((i + 2) % 4).x - verts.at(i).x);
        if((val1 < 0 && val2 > 0) || (val1 > 0 && val2 < 0))
            allOnRightSide = false;
    }
    return allOnRightSide;
}

void Rectangle::Move(sf::Vector2f val)
{
    x += val.x;
    y += val.y;
    ShapePart::Move(val);
}

ShapePart *Rectangle::MakeCopy()
{
    Rectangle *rect = new Rectangle(x, y, w, h);
    rect->density = density;
    rect->angle = angle;
    rect->collide = collide;
    rect->isStatic = isStatic;
    rect->color = color;
    rect->outline = outline;
    rect->terrain = terrain;
    rect->undragable = undragable;
    return rect;
}

void Rectangle::Init(b2World *world, b2Body *body)
{
    if(isInitted)
        return;
    ShapePart::Init(world);

    //http://www.iforce2d.net/b2dtut/fixtures
    b2PolygonShape *sd = new b2PolygonShape();
    b2FixtureDef *fd = new b2FixtureDef();
    fd->shape = sd;
    fd->friction = 0.4;
    fd->restitution = 0.3;

    fd->density = (density + 5.0) / 10.0;

    if(m_collisionGroup != -1)
        fd->filter.groupIndex = m_collisionGroup;
    if(!collide)
        fd->filter.maskBits = 0;

    VectorList verts;
    GetVertices(&verts);
    sd->Set(&verts.at(0), (int) verts.size());

    if(body)
    {
        for(unsigned i = 0; i < 4; i++)
        {
            sd->m_vertices[i].x -= body->GetPosition().x;
            sd->m_vertices[i].y -= body->GetPosition().y;
        }
        m_body = body;
        if(isStatic)
            m_body->SetType(b2_staticBody);
        //bool bodyStatic = body->GetType() == b2_staticBody;
    }
    else
    {
        for(unsigned i = 0; i < 4; i++)
        {
            sd->m_vertices[i].x -= centerX;
            sd->m_vertices[i].y -= centerY;
        }
        b2BodyDef *bd = new b2BodyDef();
        if(isStatic)
            bd->type = b2_staticBody;
        else
            bd->type = b2_dynamicBody;
        bd->position.Set(centerX, centerY);

        m_body = world->CreateBody(bd);
        if(isEditable)
        {
            bool hasJoints = false;
            for(unsigned j = 0; j < m_joints.size(); j++)
            {
                if(m_joints.at(j)->isEnabled)
                {
                    hasJoints = true;
                    break;
                }
            }
            if(!hasJoints)
                m_body->SetBullet(true);
        }
        if(isBullet)
            m_body->SetBullet(true);
    }
    //TODO:
    /*sd->userData = new Object();
    sd->userData.collide = collide;
    sd->userData.editable = (isEditable || isTank);
    sd->userData.color = color;
    sd->userData.outline = outline;
    sd->userData.terrain = terrain;
    sd->userData.undragable = undragable;
    sd->userData.isPiston = -1;
    sd->userData.isSandbox = isSandbox;*/

    m_fixture = m_body->CreateFixture(fd);

    //TODO check if mass not needed
    /*if(isStatic || bodyStatic)
        m_body->SetMass(new b2MassData());
    else
        m_body->SetMassFromShapes();*/

    for(unsigned i = 0; i < m_joints.size(); i++)
    {
        if(m_joints.at(i)->isEnabled && m_joints.at(i)->type == "FixedJoint")
        {
            ShapePart *connectedPart = m_joints.at(i)->GetOtherPart(this);
            if(connectedPart->isEnabled)
                connectedPart->Init(world, m_body);
        }
    }
}

bool Rectangle::IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH)
{
	/*if(centerX >= boxX && centerX <= boxX + boxW &&
        centerY >= boxY && centerY <= boxY + boxH)
		return true;*/
	VectorList verts;
	GetVertices(&verts);
	/*for(int i = 0; i < 4; i++) {
		if(utils.SegmentsIntersect(verts.at(i).x, verts.at(i).y, verts.at((i + 1) % 4).x,
                            verts.at((i + 1) % 4).y, boxX, boxY, boxX + boxW, boxY)) return true;
		if(utils.SegmentsIntersect(verts.at(i).x, verts.at(i).y, verts.at((i + 1) % 4).x,
                            verts.at((i + 1) % 4).y, boxX + boxW, boxY, boxX + boxW, boxY + boxH)) return true;
		if(utils.SegmentsIntersect(verts.at(i).x, verts.at(i).y, verts.at((i + 1) % 4).x,
                            verts.at((i + 1) % 4).y, boxX + boxW, boxY + boxH, boxX, boxY + boxH)) return true;
		if(utils.SegmentsIntersect(verts.at(i).x, verts.at(i).y, verts.at((i + 1) % 4).x,
                            verts.at((i + 1) % 4).y, boxX, boxY + boxH, boxX, boxY)) return true;
	}
	return false;*/
	for(int i = 0; i < 4; i++) {
        if(verts[i].x < boxX) return false;
        if(verts[i].y < boxY) return false;
        if(verts[i].x > boxX + boxW) return false;
        if(verts[i].y > boxY + boxH) return false;
	}
	return true;

	/*for(int i = 0; i < 4; i++) {
        if(verts[i].x >= boxX &&
            verts[i].y >= boxY &&
            verts[i].x <= boxX + boxW &&
            verts[i].y <= boxY + boxH) return true;
	}

	for(int i = 0; i < 4; i++) {
        if(verts[i].x <= boxX &&
            verts[i].y <= boxY &&
            verts[i].x >= boxX + boxW &&
            verts[i].y >= boxY + boxH) return true;
	}
	return false;*/

}

/*VectorList *Rectangle::GetVerticesForOutline(Num thickness)
{
    return GetOutlineVertices(GetVertices(), 4, thickness);
}*/

void Rectangle::PrepareForResizing()
{
    initW = w;
    initH = h;
}

void Rectangle::Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                     bool showJoints, bool showOutlines, sf::Texture *texture, sf::Shader *shader)
{
    if(!drawStatic && isStatic)
        return;

    window->setView(camera->view);

    if(shader)
    {
        //shader->setParameter("frag_LightOrigin", camera->ToScreenPos(sf::Vector2f(centerX, centerY)));
        //shader->setParameter("lightColor", color.ToSf());
    }

    if(m_body && m_fixture)
    {
        const b2Transform& xf = m_body->GetTransform();
        this->m_shape = m_fixture->GetShape();

        /*rectangle.setOrigin(rectangle.getSize() / 2.0f);
        rectangle.setPosition(sf::Vector2f(m_shape->, m_body->GetPosition().y));
        rectangle.setRotation(utils.RadToDeg(m_body->GetAngle()));*/

        sf::ConvexShape convex;
        b2PolygonShape *rec = (b2PolygonShape*) m_shape;
        convex.setPointCount(rec->GetVertexCount());

        // define the points
        for(int i = 0; i < rec->GetVertexCount(); i++)
        {
            b2Vec2 point = b2Mul(xf, rec->GetVertex(i));
            convex.setPoint(i, sf::Vector2f(point.x, point.y));
        }
        convex.setFillColor(color.ToSf());
        convex.setOutlineColor(color.Darken().ToSf());
        convex.setOutlineThickness(showOutlines && outline ? 0.15f : 0);
        convex.setTexture(texture);

        window->setView(camera->view);
        window->draw(convex, shader);
    }
    else
    {
        sf::Transform t = sf::Transform::Identity;
        t.rotate(angle, sf::Vector2f(centerX, centerY));

        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(w, h));
        rectangle.setFillColor(color.ToSf());
        rectangle.setOutlineColor(color.Darken().ToSf());
        rectangle.setOutlineThickness(showOutlines && outline ? 0.15f : 0);
        rectangle.setPosition(sf::Vector2f(x, y));
        rectangle.setTexture(texture);
        if(texture)
            rectangle.setTextureRect(sf::IntRect(0, 0, 250, 250));
        sf::RenderStates states;
        if(shader)
        {
            states.shader = shader;
            //states.blendMode = sf::BlendAdd;
        }

        states.transform = t;

        window->draw(rectangle, states);
    }
}

bool Rectangle::equals(ShapePart *other)
{
    return (other->type == "Rectangle" &&
            NumbersEqual(x, ((Rectangle*) other)->x) && NumbersEqual(y, ((Rectangle*) other)->y) &&
            NumbersEqual(w, ((Rectangle*) other)->w) && NumbersEqual(h, ((Rectangle*) other)->h) &&
            ShapePart::equals(other));
}

/*String Rectangle::ToString()
{
    std::stringstream s;
    s << type << std::endl << "x=" << x << ", " << "y=" << y << ", " << "w=" << w << ", " << "h=" << h << ", " << ShapePart::ToString();
    return s.str();
}*/

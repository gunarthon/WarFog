#include "Circle.h"
#include "Camera.h"

Circle::Circle(sf::Vector2f pos, Num r, bool checkLimits) : ShapePart(pos.x, pos.y)
{
    if(checkLimits)
    {
        if(r < ControllerGameSettings.minCircleRadius)
            r = ControllerGameSettings.minCircleRadius;
        if(r > ControllerGameSettings.maxCircleRadius)
            r = ControllerGameSettings.maxCircleRadius;
    }

    radius = r;
    type = "Circle";
    proprieties.list.push_back(new Propriety("radius", SLIDER, 1,  false, NULL, &radius, NULL));
}

Circle::~Circle()
{

}

Num Circle::GetArea()
{
    return PI * radius * radius;
}

bool Circle::InsideShape(sf::Vector2f val, Num scale, bool shapeOnly)
{
    return (utils.GetLength(sf::Vector2f(centerX, centerY) - val) <= radius);
}

ShapePart *Circle::MakeCopy()
{
    Circle *circ = new Circle(sf::Vector2f(centerX, centerY), radius);
    circ->density = density;
    circ->angle = angle;
    circ->collide = collide;
    circ->isStatic = isStatic;
    circ->color = color;
    circ->outline = outline;
    circ->terrain = terrain;
    circ->undragable = undragable;
    return circ;
}

void Circle::Init(b2World *world, b2Body *body)
{
    if(isInitted)
        return;
    ShapePart::Init(world);

    //http://www.iforce2d.net/b2dtut/fixtures
    b2CircleShape  *sd = new b2CircleShape ();
    sd->m_radius = radius;
    sd->m_p = b2Vec2(0,0);

    b2FixtureDef *fd = new b2FixtureDef();
    fd->shape = sd;
    fd->friction = 0.7;
    fd->restitution = 0.3;

    fd->density = (density + 5.0) / 10.0;

    if(m_collisionGroup != -1)
        fd->filter.groupIndex = m_collisionGroup;
    if(!collide)
        fd->filter.maskBits = 0;

    if(body)
    {
        sd->m_p = b2Vec2(centerX - body->GetPosition().x, centerY - body->GetPosition().y);
        m_body = body;
        if(isStatic)
            m_body->SetType(b2_staticBody);
    }
    else
    {
        b2BodyDef *bd = new b2BodyDef();
        if(isStatic)
            bd->type = b2_staticBody;
        else
            bd->type = b2_dynamicBody;
        bd->angularDamping = 0.4f * fd->friction;
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

bool Circle::IntersectsBox(Num boxX, Num boxY, Num boxW, Num boxH)
{
    if(centerX >= boxX && centerX <= boxX + boxW && centerY >= boxY && centerY <= boxY + boxH)
        return true;

    if(centerX >= boxX && centerX <= boxX + boxW)
    {
        if(utils.Abs(boxY - centerY) <= radius)
            return true;
        if(utils.Abs(boxY + boxH - centerY) <= radius)
            return true;
    }

    if(centerY >= boxY && centerY <= boxY + boxH) {
        if(utils.Abs(boxX - centerX) <= radius)
            return true;
        if(utils.Abs(boxX + boxW - centerX) <= radius)
            return true;
    }

    if(utils.GetLength(sf::Vector2f(centerX, centerY) - sf::Vector2f(boxX, boxY)) < radius)
        return true;
    if(utils.GetLength(sf::Vector2f(centerX, centerY) - sf::Vector2f(boxX + boxW, boxY)) < radius)
        return true;
    if(utils.GetLength(sf::Vector2f(centerX, centerY) - sf::Vector2f(boxX + boxW, boxY + boxH)) < radius)
        return true;
    if(utils.GetLength(sf::Vector2f(centerX, centerY) - sf::Vector2f(boxX, boxY + boxH)) < radius)
        return true;

    return false;
}

void Circle::PrepareForResizing()
{
    initRadius = radius;
}

bool Circle::equals(ShapePart *other)
{
    return (other->type == "Circle" &&
            NumbersEqual(radius, ((Circle*) other)->radius) &&
            ShapePart::equals(other));
}

void Circle::Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
          bool showJoints, bool showOutlines,
          sf::Texture *texture, sf::Shader *shader)
{
    if(!drawStatic && isStatic)
        return;

    window->setView(camera->view);

    if(shader)
    {
        //shader->setParameter("frag_LightOrigin", camera->ToScreenPos(sf::Vector2f(centerX, centerY)));
        //shader->setParameter("lightColor", color.ToSf());
    }

    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setOrigin(radius, radius);
    circle.setFillColor(color.ToSf());
    circle.setOutlineColor(color.Darken().ToSf());
    circle.setOutlineThickness(showOutlines && outline ? 0.15f : 0);
    sf::Transform t = sf::Transform::Identity;

    if(m_body && m_fixture)
    {
        const b2Transform& xf = m_body->GetTransform();
        this->m_shape = m_fixture->GetShape();

        /*rectangle.setOrigin(rectangle.getSize() / 2.0f);
        rectangle.setPosition(sf::Vector2f(m_shape->, m_body->GetPosition().y));
        rectangle.setRotation(utils.RadToDeg(m_body->GetAngle()));*/

        b2CircleShape *cir = (b2CircleShape*) m_shape;
        b2Vec2 point = b2Mul(xf, b2Vec2(cir->GetVertex(0)));
        circle.setPosition(point.x, point.y);
    }
    else
    {
        circle.setPosition(sf::Vector2f(centerX, centerY));
    }

    circle.setTexture(texture);
    if(texture)
        circle.setTextureRect(sf::IntRect(0, 0, 250, 250));
    sf::RenderStates states;
    if(shader)
    {
        states.shader = shader;
        //states.blendMode = sf::BlendAdd;
    }

    states.transform = t;

    window->draw(circle, states);
}

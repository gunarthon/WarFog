#include "Thruster.h"
#include "ShapePart.h"
#include "Camera.h"

Thruster::Thruster(ShapePart *p1, sf::Vector2f pos, bool valid) : JointPart(p1, NULL)
{
    anchor = pos;
    type = "Thruster";
    this->valid = valid;

    if(ControllerGameSettings.maxThrusterStrength < 15.0)
        strength = ControllerGameSettings.maxThrusterStrength;
	else
		strength = 15.0;

    angle = -90.0f;

    isKeyDown = false;
    thrustKey = sf::Keyboard::W;

    autoOn = false;
    isBalloon = false;


    proprieties.list.push_back(new Propriety("autoOn", BOOL, 0, true, &autoOn));
    proprieties.list.push_back(new Propriety("balloon", BOOL, 0, true, &isBalloon));

    proprieties.list.push_back(new Propriety("angle", SLIDER, 1, true, NULL, &angle, NULL, -360, 360));
    proprieties.list.push_back(new Propriety("strength", SLIDER, 1, true, NULL, &strength, NULL, 0, ControllerGameSettings.maxThrusterStrength));
    proprieties.list.push_back(new Propriety("input", sf::Keyboard::W, true, &thrustKey));
}

Thruster::~Thruster()
{

}

Part *Thruster::MakeCopy()
{
    Thruster *t = new Thruster(NULL, anchor);
    t->strength = strength;
    t->autoOn = autoOn;
    t->thrustKey = thrustKey;
    t->angle = angle;
    return t;
}

bool Thruster::InsideShape(sf::Vector2f val, Num scale, bool shapeOnly)
{
    if(shapeOnly)
        return false;
    return (utils.GetLength(anchor - val) < 5.64f / scale);
}

void Thruster::Init(b2World *world, b2Body *body)
{
    if(isInitted)
        return;
    JointPart::Init(world);
    isKeyDown = false;
    relativeThrusterPos = b2Vec2(anchor.x, anchor.y);
    relativeThrusterPos -= part1->GetBody()->GetPosition();
}

void Thruster::KeyInput(sf::Keyboard::Key key, bool up, bool replay)
{
    if(key == thrustKey)
        isKeyDown = !up;
}

void Thruster::Update(b2World *world, InputManager *input)
{
    isKeyDown = input->KeyDown(thrustKey);
    if(isInitted && (isKeyDown || autoOn))
    {
        Num forceAngle;
        if(isBalloon)
            forceAngle = -PI / 2;
        else
            forceAngle = utils.DegToRad(angle) + part1->GetBody()->GetAngle();

        Num forceStrength = strength * 10;

        b2Vec2 forceVector = b2Vec2(std::cos(forceAngle) * forceStrength, std::sin(forceAngle) * forceStrength);
        b2Vec2 positionVector = part1->GetBody()->GetWorldPoint(relativeThrusterPos);
        part1->GetBody()->ApplyForce(forceVector, positionVector, true);
    }
}

void Thruster::Move(sf::Vector2f val)
{
    anchor += val;
}

void Thruster::RotateAround(sf::Vector2f center, Num curAngle)
{
    /*var dist:Number = Util.GetDist(centerX, centerY, xVal, yVal);
			var absoluteAngle:Number = rotateAngle + curAngle;
			Move(xVal + dist * Math.cos(absoluteAngle), yVal + dist * Math.sin(absoluteAngle));
			angle = curAngle + rotateOrientation;*/
}

/*PartList *Thruster::GetAttachedParts(PartList *partList)
{

}*/


void Thruster::PrepareForResizing()
{

}

void Thruster::Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                    bool showJoints, bool showOutlines, sf::Texture *texture, sf::Shader *shader)
{
    if(!showJoints)
        return;

    if(!drawStatic && isStatic)
        return;

    sf::ConvexShape convex;
    convex.setPointCount(3);

    convex.setPoint(0, sf::Vector2f(0, -5.64f / camera->GetScale()));
    convex.setPoint(1, sf::Vector2f(-5.64f / camera->GetScale(), 5.64f / camera->GetScale()));
    convex.setPoint(2, sf::Vector2f(5.64f / camera->GetScale(), 5.64f / camera->GetScale()));
    convex.setFillColor(sf::Color::Transparent);
    if(!valid)
        convex.setOutlineColor(sf::Color::Red);
    else
        convex.setOutlineColor(autoOn ? sf::Color::Magenta : sf::Color::Yellow);
    convex.setOutlineThickness(1 / camera->GetScale());

    convex.setOrigin(0, 0);
    convex.rotate(angle + 90.0f);

    if(m_joint)
    {

    }
    else
    {
        convex.setPosition(anchor);

        if(texture)
            convex.setTextureRect(sf::IntRect(0, 0, 250, 250));
        sf::RenderStates states;
        if(shader)
        {
            states.shader = shader;
        }

        window->setView(camera->view);
        window->draw(convex, states);
    }
}

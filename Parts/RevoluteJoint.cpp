#include "RevoluteJoint.h"
#include "ShapePart.h"
#include "Camera.h"

RevoluteJoint::RevoluteJoint(ShapePart *p1, ShapePart *p2, sf::Vector2f pos, bool valid) : JointPart(p1, p2)
{
    anchor = pos;
    this->valid = valid;
    enableMotor = false;
    isStiff = false;
    autoCW = false,
    autoCCW = false;
    hasLowerLimit = false;
    hasUpperLimit = false;

    isKeyDown1 = false;
    isKeyDown2 = false;
    wasKeyDown1 = false;
    wasKeyDown2 = false;

    motorCWKey = sf::Keyboard::D;
    motorCCWKey = sf::Keyboard::A;
    if(ControllerGameSettings.maxRJStrength < 15.0)
        motorStrength = ControllerGameSettings.maxRJStrength;
    else
        motorStrength = 15.0;
    if(ControllerGameSettings.maxRJSpeed < 15.0)
        motorSpeed = ControllerGameSettings.maxRJSpeed;
    else
        motorSpeed = 15.0;

    motorLowerLimit = 0;
    motorUpperLimit = 90;
    type = "RevoluteJoint";

    proprieties.list.push_back(new Propriety("enableMotor", BOOL, 0, true, &enableMotor));
    proprieties.list.push_back(new Propriety("isStiff", BOOL, 1, true, &isStiff));
    proprieties.list.push_back(new Propriety("autoCW", BOOL, 0, true, &autoCW));
    proprieties.list.push_back(new Propriety("autoCCW", BOOL, 0, true, &autoCCW));
    proprieties.list.push_back(new Propriety("hasLowLimit", BOOL, 1, true, &hasLowerLimit));
    proprieties.list.push_back(new Propriety("hasUpLimit", BOOL, 1, true, &hasUpperLimit));

    proprieties.list.push_back(new Propriety("Strength", SLIDER, 1, true, NULL, &motorStrength, NULL, 0, ControllerGameSettings.maxRJStrength));
    proprieties.list.push_back(new Propriety("Speed", SLIDER, 1, true, NULL, &motorSpeed, NULL, 0, ControllerGameSettings.maxRJSpeed));
    proprieties.list.push_back(new Propriety("LowLimit", SLIDER, 1, true, NULL, &motorLowerLimit, NULL, -360, 360));
    proprieties.list.push_back(new Propriety("UpLimit", SLIDER, 1, true, NULL, &motorUpperLimit, NULL, -360, 360));

    proprieties.list.push_back(new Propriety("inputCW", sf::Keyboard::D, true, &motorCWKey));
    proprieties.list.push_back(new Propriety("inputCCW", sf::Keyboard::A, true, &motorCCWKey));
}

RevoluteJoint::~RevoluteJoint()
{

}


void RevoluteJoint::SetJointProperties(RevoluteJoint *other)
{
    enableMotor = other->enableMotor;
    motorCWKey = other->motorCWKey;
    motorCCWKey = other->motorCCWKey;
    motorStrength = other->motorStrength;
    motorSpeed = other->motorSpeed;
    hasLowerLimit = other->hasLowerLimit;
    hasUpperLimit = other->hasUpperLimit;
    motorLowerLimit = other->motorLowerLimit;
    motorUpperLimit = other->motorUpperLimit;
    isStiff = other->isStiff;
    autoCW = other->autoCW;
    autoCCW = other->autoCCW;
}

Part *RevoluteJoint::MakeCopy()
{
    RevoluteJoint *j = new RevoluteJoint(NULL, NULL, anchor);
    j->enableMotor = enableMotor;
    j->motorCWKey = motorCWKey;
    j->motorCCWKey = motorCCWKey;
    j->motorStrength = motorStrength;
    j->motorSpeed = motorSpeed;
    j->hasLowerLimit = hasLowerLimit;
    j->hasUpperLimit = hasUpperLimit;
    j->motorLowerLimit = motorLowerLimit;
    j->motorUpperLimit = motorUpperLimit;
    j->isStiff = isStiff;
    j->autoCW = autoCW;
    j->autoCCW = autoCCW;
    return j;
}

void RevoluteJoint::Init(b2World *world, b2Body *body)
{
    if(isInitted)
        return;
    JointPart::Init(world);

    if(part1->GetBody() != part2->GetBody())
    {
        static Utils utils;

        b2RevoluteJointDef *jd = new b2RevoluteJointDef();
        jd->enableMotor = enableMotor;

        jd->maxMotorTorque = motorStrength * 30;

        jd->enableLimit = (hasLowerLimit || hasUpperLimit);

        if(!hasLowerLimit)
            jd->lowerAngle = -std::numeric_limits<Num>::max();
        else
            jd->lowerAngle = utils.DegToRad(motorLowerLimit);

        if(!hasUpperLimit)
            jd->upperAngle = std::numeric_limits<Num>::max();
        else
            jd->upperAngle = utils.DegToRad(motorUpperLimit);

        if(part1->type == "Circle" && part2->type != "Circle")
            jd->Initialize(part2->GetBody(), part1->GetBody(), b2Vec2(anchor.x, anchor.y));
        else if(part1->type != "Circle" && part2->type == "Circle")
            jd->Initialize(part1->GetBody(), part2->GetBody(), b2Vec2(anchor.x, anchor.y));
        else if(part1->HeavierThan(part2))
            jd->Initialize(part1->GetBody(), part2->GetBody(), b2Vec2(anchor.x, anchor.y));
        else
            jd->Initialize(part2->GetBody(), part1->GetBody(), b2Vec2(anchor.x, anchor.y));

        m_joint = world->CreateJoint(jd);
        targetJointAngle = 0;

        isKeyDown1 = false;
        isKeyDown2 = false;
        wasKeyDown1 = false;
        wasKeyDown2 = false;
    }
}

void RevoluteJoint::CheckForBreakage(b2World *world)
{
    if(m_joint)
    {
        b2RevoluteJoint *joint = (b2RevoluteJoint*) m_joint;

        // Check joint constraints to see if the joint should break
        static Utils utils;
        Num dist2 = utils.GetLength2(sf::Vector2f(joint->GetAnchorA().x, joint->GetAnchorA().y) -
                                     sf::Vector2f(joint->GetAnchorB().x, joint->GetAnchorB().y));
        if (dist2 > 9.0) {
            world->DestroyJoint(m_joint);
            m_joint = NULL;
        }
    }
}

void RevoluteJoint::Update(b2World *world, InputManager *input)
{
    if(m_joint && enableMotor)
    {
        isKeyDown1 = input->KeyDown(motorCWKey);
        isKeyDown2 = input->KeyDown(motorCCWKey);

        b2RevoluteJoint *joint = (b2RevoluteJoint*) m_joint;
        if(isKeyDown1 || isKeyDown2)
        {
            joint->EnableMotor(true);
            joint->SetMaxMotorTorque(motorStrength * 30);

            part1->GetBody()->SetAwake(true);
            part2->GetBody()->SetAwake(true);
        }

        if(isKeyDown1 || (autoCW && !isKeyDown2))
        {
            joint->SetMotorSpeed(motorSpeed);

            if(isStiff && wasKeyDown1 && joint->GetJointAngle() < prevJointAngle)
            {
                joint->SetMotorSpeed(0);
                joint->SetMaxMotorTorque(motorStrength * 3000);

            }
        }
        else if(isKeyDown2 || autoCCW)
        {
            joint->SetMotorSpeed(0.0 - motorSpeed);

            if(isStiff && wasKeyDown2 && joint->GetJointAngle() > prevJointAngle)
            {
                joint->SetMotorSpeed(0);
                joint->SetMaxMotorTorque(motorStrength * 3000);
            }
        }
        else
        {
            if(wasKeyDown1 || wasKeyDown2)
                targetJointAngle = joint->GetJointAngle();

            joint->EnableMotor(isStiff);
            joint->SetMotorSpeed(0);

            joint->SetMaxMotorTorque(motorStrength * 3000);

        }

        wasKeyDown1 = isKeyDown1;
        wasKeyDown2 = isKeyDown2;
        prevJointAngle = joint->GetJointAngle();
    }
}

bool RevoluteJoint::InsideShape(sf::Vector2f val, Num scale, bool shapeOnly)
{
    if(shapeOnly)
        return false;
    return (utils.GetLength(anchor - val) < 5.64f / scale);
}

void RevoluteJoint::KeyInput(sf::Keyboard::Key key, bool up, bool replay)
{
    if(key == motorCWKey)
        isKeyDown1 = !up;
    if(key == motorCCWKey)
        isKeyDown2 = !up;
}

/*String RevoluteJoint::ToString()
{
    std::stringstream s;
    s << "RevoluteJoint: enableMotor=" << enableMotor << ", motorCWKey=" << motorCWKey <<
    ", motorCCWKey=" << motorCCWKey << ", motorStrength=" << motorStrength << ", motorSpeed=" << motorSpeed <<
    ", hasLowerLimit=" << hasLowerLimit << ", hasUpperLimit=" << hasUpperLimit <<
    ", motorLowerLimit=" << motorLowerLimit << ", motorUpperLimit=" << motorUpperLimit <<
    ", isStiff=" << isStiff << ", " << JointPart::ToString();

    return s.str();
}*/

void RevoluteJoint::Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                         bool showJoints, bool showOutlines, sf::Texture *texture, sf::Shader *shader)
{
    if(!showJoints)
        return;

    if(!drawStatic && isStatic)
        return;

    sf::CircleShape circle;
    circle.setRadius(5.64f / camera->GetScale());
    circle.setFillColor(sf::Color::Transparent);
    if(!valid)
        circle.setOutlineColor(sf::Color::Red);
    else
        circle.setOutlineColor(enableMotor ? sf::Color::Magenta : sf::Color::Yellow);
    circle.setOutlineThickness(1 / camera->GetScale());

    circle.setOrigin(circle.getRadius(), circle.getRadius());

    if(m_joint)
    {

    }
    else
    {
        circle.setPosition(anchor);
    }

    if(texture)
        circle.setTextureRect(sf::IntRect(0, 0, 250, 250));
    sf::RenderStates states;
    if(shader)
    {
        states.shader = shader;
    }

    window->setView(camera->view);
    window->draw(circle, states);
}

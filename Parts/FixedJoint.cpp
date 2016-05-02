#include "FixedJoint.h"
#include "ShapePart.h"
#include "Camera.h"

FixedJoint::FixedJoint(ShapePart *p1, ShapePart *p2, sf::Vector2f pos, bool valid) : JointPart(p1, p2)
{
    anchor = pos;
    this->valid = valid;
    type = "FixedJoint";
}

FixedJoint::~FixedJoint()
{
    //dtor
}

Part *FixedJoint::MakeCopy()
{
    return new FixedJoint(NULL, NULL, anchor);
}

void FixedJoint::Update(b2World *world, InputManager *input)
{

}

bool FixedJoint::InsideShape(sf::Vector2f val, Num scale, bool shapeOnly)
{
    if(shapeOnly)
        return false;
    static Utils utils;
    return (utils.GetLength(anchor - val) < 0.18 * 30 / scale);
}

void FixedJoint::Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
                      bool showJoints, bool showOutlines, sf::Texture *texture, sf::Shader *shader)
{
    if(!showJoints)
        return;

    if(!drawStatic && isStatic)
        return;

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(10, 10) / camera->GetScale());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(valid ? sf::Color::Yellow : sf::Color::Red);
    rectangle.setOutlineThickness(1 / camera->GetScale());

    rectangle.setOrigin(rectangle.getSize() / 2.0f);

    if(m_joint)
    {

    }
    else
    {
        rectangle.setPosition(anchor);
    }

    if(texture)
        rectangle.setTextureRect(sf::IntRect(0, 0, 250, 250));
    sf::RenderStates states;
    if(shader)
    {
        states.shader = shader;
    }
    window->setView(camera->view);
    window->draw(rectangle, states);
}

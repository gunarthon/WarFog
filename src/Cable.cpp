#include "Cable.h"
#include "ShapePart.h"
#include "Camera.h"

Cable::Cable(ShapePart *p1, ShapePart *p2, sf::Vector2f pos, int index1, int index2, bool valid) : JointPart(p1, p2)
{
    anchor = pos;
    this->valid = valid;
    type = "Cable";
    this->color = Color(10,0,255,100);
    this->index1 = index1;
    this->index2 = index2;
}

Cable::~Cable()
{

}

void Cable::Update(b2World *world, InputManager *input)
{
    if(part1->GetOutput(index1))
        this->color = Color(255,0,10,100);
    else
        this->color = Color(10,0,255,100);
}

bool Cable::InsideShape(sf::Vector2f val, Num scale, bool shapeOnly)
{
    return false;
}

void Cable::Draw(sf::RenderWindow *window, Camera *camera, bool drawStatic,
    bool showJoints, bool showOutlines,
    sf::Texture *texture, sf::Shader *shader)
{
    if(!drawStatic && isStatic)
        return;

    if(!part1)
        return;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(part1->centerX, part1->centerY), color.ToSf()),
        sf::Vertex(part2 ? sf::Vector2f(part2->centerX, part2->centerY) : anchor, color.ToSf())
    };


    sf::RenderStates states;
    if(shader)
    {
        states.shader = shader;
    }
    window->setView(camera->view);
    //window->draw(line, states);
    window->draw(line, 2, sf::Lines);
}

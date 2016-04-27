#include "Controller.h"
#include "InputManager.h"

Controller::Controller()
{
    mouseDown = false;
    input = NULL;
    exit = false;
}

Controller::~Controller()
{

}

void Controller::SetInput(InputManager *input)
{
    this->input = input;
}

bool Controller::KeyPress(sf::Keyboard::Key key)
{
    if(key != sf::Keyboard::Escape && key != sf::Keyboard::Return && key != sf::Keyboard::Space)
        return interface.Input(key);
    return false;
}

bool Controller::KeyRelease(sf::Keyboard::Key key)
{
    return false;
}

bool Controller::MouseDown(sf::Mouse::Button button, Num x, Num y)
{
    mouseDown = true;
    mouseX = x;
    mouseY = y;
    mouseInitDragX = x;
    mouseInitDragY = y;
    return interface.MouseDown(button, sf::Vector2f(x,y));
}

bool Controller::MouseUp(sf::Mouse::Button button, Num x, Num y)
{
    mouseDown = false;
    mouseX = x;
    mouseY = y;
    mouseDragX = x - mouseInitDragX;
    mouseDragY = y - mouseInitDragY;
    return interface.MouseUp(button, sf::Vector2f(x,y));
}

bool Controller::MouseMove(Num x, Num y)
{
    mouseX = x;
    mouseY = y;
    return interface.MouseMove(sf::Vector2f(x,y));
}

bool Controller::MouseWheel(sf::Vector2f pos, Num delta)
{
    mouseWheelVal += delta;
    mouseX = pos.x;
    mouseY = pos.y;
    return false;
}
bool Controller::TextEntered(char text)
{
    return interface.TextEntered(text);
}
bool Controller::GetExit()
{
    return exit;
}

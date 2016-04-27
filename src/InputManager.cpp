#include "InputManager.h"

InputManager::InputManager()
{
    controller = NULL;
}

InputManager::~InputManager()
{
    //dtor
}

void InputManager::SetController(Controller *controller)
{
    this->controller = controller;
    if(controller)
        controller->SetInput(this);
}

void InputManager::Update(sf::Event event, sf::RenderWindow &window)
{
    if(!controller)
        return;

    this->event = event;
    if(event.type == sf::Event::KeyPressed)
        controller->KeyPress(event.key.code);

    else if(event.type == sf::Event::KeyReleased)
        controller->KeyRelease(event.key.code);

    else if(event.type == sf::Event::MouseButtonPressed)
        controller->MouseDown(event.mouseButton.button, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

    else if(event.type == sf::Event::MouseButtonReleased)
        controller->MouseUp(event.mouseButton.button, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

    else if(event.type == sf::Event::MouseMoved)
        controller->MouseMove(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

    else if(event.type == sf::Event::MouseWheelMoved)
        controller->MouseWheel(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y), event.mouseWheel.delta);

    else if(event.type == sf::Event::TextEntered)
    {
        if(event.text.unicode < 128)
            controller->TextEntered(static_cast<char>(event.text.unicode));
    }
    else if(event.type == sf::Event::Resized)
    {
        window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
    }

}

bool InputManager::KeyPressed(sf::Keyboard::Key key)
{
    if(event.key.code == key && event.type == sf::Event::KeyPressed)
        return true;
    return false;
}

bool InputManager::KeyPressed(std::vector<sf::Keyboard::Key> keys)
{
    for(unsigned i = 0; i < keys.size(); i++)
    {
        if(KeyPressed(keys[i]))
            return true;
    }
    return false;
}

bool InputManager::KeyReleased(sf::Keyboard::Key key)
{
    if(event.key.code == key && event.type == sf::Event::KeyReleased)
        return true;
    return false;
}

bool InputManager::KeyReleased(std::vector<sf::Keyboard::Key> keys)
{
    for(unsigned i = 0; i < keys.size(); i++)
    {
        if(KeyReleased(keys[i]))
            return true;
    }
    return false;
}

bool InputManager::KeyDown(sf::Keyboard::Key key)
{
    if(sf::Keyboard::isKeyPressed(key))
        return true;
    return false;
}

bool InputManager::KeyDown(std::vector<sf::Keyboard::Key> keys)
{
    for(unsigned i = 0; i < keys.size(); i++)
    {
        if(sf::Keyboard::isKeyPressed(keys[i]))
            return true;
    }
    return false;
}

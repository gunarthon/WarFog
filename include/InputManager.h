#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Controller.h"

class InputManager
{
    public:
        InputManager();
        virtual ~InputManager();

        void SetController(Controller *controller);
        void Update(sf::Event event, sf::RenderWindow &window);

        bool KeyPressed(sf::Keyboard::Key key);
        bool KeyPressed(std::vector<sf::Keyboard::Key> keys);

        bool KeyReleased(sf::Keyboard::Key key);
        bool KeyReleased(std::vector<sf::Keyboard::Key> keys);

        bool KeyDown(sf::Keyboard::Key key);
        bool KeyDown(std::vector<sf::Keyboard::Key> keys);
    protected:

    private:
        sf::Event event;
        Controller *controller;
};

#endif // INPUTMANAGER_H

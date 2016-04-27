#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Utils.h"
#include "FileManager.h"
#include "GUI.h"
#include "InterfaceManager.h"

class InputManager;

class Controller
{
    public:
        Controller();
        virtual ~Controller();

        void SetInput(InputManager *input);

        virtual bool KeyPress(sf::Keyboard::Key key);
        virtual bool KeyRelease(sf::Keyboard::Key key);
        virtual bool MouseDown(sf::Mouse::Button button, Num x, Num y);
        virtual bool MouseUp(sf::Mouse::Button button, Num x, Num y);
        virtual bool MouseMove(Num x, Num y);
        virtual bool MouseWheel(sf::Vector2f pos, Num delta);
        virtual bool TextEntered(char text);
        bool GetExit();


        InterfaceManager interface;

    protected:
        bool mouseDown;
        Num mouseX;
        Num mouseY;
        Num mouseOffsetX;
        Num mouseOffsetY;
        Num mouseWheelVal;
        Num mouseDragX;
        Num mouseDragY;
        Num mouseInitDragX;
        Num mouseInitDragY;

        InputManager *input;
        Utils utils;

        FileManager file;
        bool exit;
    private:
};

#endif // CONTROLLER_H

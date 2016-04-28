#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include "GUI.h"
#include "PartList.h"
#include "PartsGUI.h"
#include "SaveGUI.h"
#include "LoadGUI.h"
#include "MainMenuGUI.h"
#include "ToolsGUI.h"

class InterfaceManager
{
    public:
        InterfaceManager();
        virtual ~InterfaceManager();

        virtual void Update();
        virtual void Draw(sf::RenderWindow *window, sf::Font *font);
        virtual void ChangeFocus(GUI *focus = NULL);
        virtual void Show(GUI *gui);
        virtual void Hide(GUI *gui = NULL);

        virtual bool MouseDown(sf::Mouse::Button button, sf::Vector2f mouse);
        virtual bool MouseUp(sf::Mouse::Button button, sf::Vector2f mouse);
        virtual bool MouseMove(sf::Vector2f mouse);
        virtual bool TextEntered(char text);

        virtual bool Input(sf::Keyboard::Key i);

        GuiList list;

    protected:

    private:
        int currentFocus;
        int stealFocus;
};

#endif // INTERFACEMANAGER_H

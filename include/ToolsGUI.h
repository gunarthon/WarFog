#ifndef TOOLSGUI_H
#define TOOLSGUI_H

#include "GUI.h"

class ToolsGUI : public GUI
{
    public:
        ToolsGUI(String &title, std::function<void (ActionId s)> changeActionCallback, sf::Vector2f offset = sf::Vector2f(10,10));
        virtual ~ToolsGUI();

        bool MouseDown(sf::Vector2f mouse);
    protected:
        std::function<void (ActionId)> changeActionCallback;

    private:
};

#endif // TOOLSGUI_H

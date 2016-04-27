#ifndef LOADGUI_H
#define LOADGUI_H

#include "GUI.h"


class LoadGUI : public GUI
{
    public:
        LoadGUI(String &title, std::function<void (String s)> loadCallback, sf::Vector2f offset = sf::Vector2f(0,0));
        virtual ~LoadGUI();

        bool MouseDown(sf::Vector2f mouse);

    protected:
        std::function<void (String s)> loadCallback;
        String fileName;
};

#endif // LOADGUI_H

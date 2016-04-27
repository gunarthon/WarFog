#ifndef SAVEGUI_H
#define SAVEGUI_H

#include "GUI.h"
class ControllerGame;


class SaveGUI : public GUI
{
    public:
        SaveGUI(String &title, std::function<void (String s)> saveCallback, sf::Vector2f pos = sf::Vector2f(0,0));
        virtual ~SaveGUI();

        bool MouseDown(sf::Vector2f mouse);

    protected:
        std::function<void (String s)> saveCallback;
        String fileName;
    private:
};

#endif // SAVEGUI_H

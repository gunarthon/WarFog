#ifndef DEVELOPERGUI_H
#define DEVELOPERGUI_H

#include "GUI.h"


class DeveloperGUI : public GUI
{
    public:
        DeveloperGUI(String &title, Proprieties *proprieties, sf::Vector2f offset = sf::Vector2f(10,10));
        virtual ~DeveloperGUI();

    protected:

    private:
};

#endif // DEVELOPERGUI_H

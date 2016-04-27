#include "DeveloperGUI.h"

DeveloperGUI::DeveloperGUI(String &title, Proprieties *proprieties, sf::Vector2f offset) : GUI(title, offset)
{
    this->size = sf::Vector2f(210, 200);
    this->proprieties = proprieties;
    this->hide = false;
    this->focus = false;

    this->align = ALIGN_DOWN_RIGHT;

    selected = -1;
}

DeveloperGUI::~DeveloperGUI()
{
    //dtor
}

#include "ToolsGUI.h"

ToolsGUI::ToolsGUI(String &title, std::function<void (ActionId)> changeActionCallback, sf::Vector2f offset) : GUI(title, offset)
{
    this->size = sf::Vector2f(210, 400);
    this->proprieties = new Proprieties();
    this->changeActionCallback = changeActionCallback;

    this->align = ALIGN_UP_RIGHT;

    proprieties->list.push_back(new Propriety("Rectangle", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Circle", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Triangle", BUTTON, 1,  false, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Weld", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Axis", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Thruster", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Piston", BUTTON, 1,  false, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Delete", BUTTON, 1,  false, NULL, NULL, NULL));

    selected = -1;
}

ToolsGUI::~ToolsGUI()
{
    delete proprieties;
}

bool ToolsGUI::MouseDown(sf::Vector2f mouse)
{
    if(hide)
        return false;

    GUI::MouseDown(mouse);

    if(selected >= (int) proprieties->list.size())
        return false;

    if(selected < 0)
        return false;

    if(selected == 0)
        changeActionCallback(NEW_RECT);
    else if(selected == 1)
        changeActionCallback(NEW_CIRCLE);
    else if(selected == 2)
        changeActionCallback(NEW_TRIANGLE);
    else if(selected == 3)
        changeActionCallback(NEW_FIXED_JOINT);
    else if(selected == 4)
        changeActionCallback(NEW_REVOLUTE_JOINT);
    else if(selected == 5)
        changeActionCallback(NEW_THRUSTER);
    else if(selected == 6)
        changeActionCallback(NEW_PISTON);

    return true;
}

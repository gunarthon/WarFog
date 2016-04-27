#include "SaveGUI.h"
#include "Controller.h"

SaveGUI::SaveGUI(String &title, std::function<void (String s)> saveCallback, sf::Vector2f offset) : GUI(title, offset)
{
    this->saveCallback = saveCallback;
    this->size = sf::Vector2f(210, 110);
    this->showOnTop = true;
    this->stealFocus = true;

    this->proprieties = new Proprieties();

    this->align = ALIGN_CENTER;
    fileName = "design";
    proprieties->list.push_back(new Propriety("File name:", STRING, 1,  true, NULL, NULL, &fileName));
    proprieties->list.push_back(new Propriety("Save", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Cancel", BUTTON, 1,  true, NULL, NULL, NULL));

    selected = 0;
}

SaveGUI::~SaveGUI()
{
    delete proprieties;
}

bool SaveGUI::MouseDown(sf::Vector2f mouse)
{
    if(hide)
        return false;

    GUI::MouseDown(mouse);

    if(selected >= (int) proprieties->list.size())
        return false;

    if(selected < 0)
        return false;

    if(selected == 1)
    {
        saveCallback(fileName);
        Hide();
    }
    else if(selected == 2)
        Hide();
    return false;
}

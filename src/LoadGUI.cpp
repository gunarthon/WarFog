#include "LoadGUI.h"

#include "Controller.h"

LoadGUI::LoadGUI(String &title, std::function<void (String s)> loadCallback, sf::Vector2f offset) : GUI(title, offset)
{
    this->loadCallback = loadCallback;
    this->size = sf::Vector2f(210, 110);
    this->showOnTop = true;
    this->stealFocus = true;

    this->proprieties = new Proprieties();

    this->align = ALIGN_CENTER;
    fileName = "design";
    proprieties->list.push_back(new Propriety("File name:", STRING, 1,  true, NULL, NULL, &fileName));
    proprieties->list.push_back(new Propriety("Load", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Cancel", BUTTON, 1,  true, NULL, NULL, NULL));

    selected = 0;
}

LoadGUI::~LoadGUI()
{
    delete proprieties;
}

bool LoadGUI::MouseDown(sf::Vector2f mouse)
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
        loadCallback(fileName);
        Hide();
    }
    else if(selected == 2)
        Hide();

    return false;
}

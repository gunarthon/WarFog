#include "PartsGUI.h"
#include "Part.h"

PartsGUI::PartsGUI(String &title, PartList *allParts, sf::Vector2f offset) : GUI(title, offset)
{
    partList = allParts;
    this->size = sf::Vector2f(210, 400);
}

PartsGUI::~PartsGUI()
{
    //dtor
}

void PartsGUI::Draw(sf::RenderWindow *window, sf::Font *font)
{
    if(hide || !partList->selected.size())
        return;

    proprieties = &(partList->At(partList->selected[0])->proprieties);
    GUI::Draw(window, font);
}

bool PartsGUI::MouseDown(sf::Vector2f mouse)
{
    focus = false;
    if(hide || !partList->selected.size())
        return false;

    proprieties = &(partList->At(partList->selected[0])->proprieties);
    return GUI::MouseDown(mouse);
}

bool PartsGUI::MouseMove(sf::Vector2f mouse)
{
    if(hide || !partList->selected.size())
        return false;

    proprieties = &(partList->At(partList->selected[0])->proprieties);
    GUI::MouseMove(mouse);
    return false;
}

bool PartsGUI::Input(sf::Keyboard::Key i)
{
    if(hide || !partList->selected.size())
        return false;

    proprieties = &(partList->At(partList->selected[0])->proprieties);
    return GUI::Input(i);
}

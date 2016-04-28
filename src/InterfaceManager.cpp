#include "InterfaceManager.h"

InterfaceManager::InterfaceManager()
{
    currentFocus = -1;
    stealFocus = -1;
}

InterfaceManager::~InterfaceManager()
{
    for(unsigned i = 0; i < list.size(); i++)
        delete list[i];
}

void InterfaceManager::Update()
{
    currentFocus = -1;
    stealFocus = -1;
    for(unsigned i = 0; i < list.size(); i++)
        if(list[i]->HasFocus())
        {
            currentFocus = i;
            if(list[i]->stealFocus)
            {
                stealFocus = i;
                break;
            }

        }
}

void InterfaceManager::Draw(sf::RenderWindow *window, sf::Font *font)
{
    for(unsigned i = 0; i < list.size(); i++)
        list[i]->Draw(window, font);
}


bool InterfaceManager::MouseDown(sf::Mouse::Button button, sf::Vector2f mouse)
{
    currentFocus = -1;
    if(stealFocus == -1)
    {
        for(unsigned i = 0; i < list.size(); i++)
        {
            if(list[i]->InsideGUI(mouse))
            {
                currentFocus = i;
                if(list[i]->stealFocus)
                {
                    stealFocus = i;
                    break;
                }
            }
        }
    }
    else
        currentFocus = stealFocus;

    if(currentFocus != -1)
        return list[currentFocus]->MouseDown(mouse);


    return false;
}

bool InterfaceManager::MouseUp(sf::Mouse::Button button, sf::Vector2f mouse)
{
    if(currentFocus != -1)
        return list[currentFocus]->MouseUp(mouse);
    return false;
}

bool InterfaceManager::MouseMove(sf::Vector2f mouse)
{
    if(stealFocus == -1)
    {
        for(unsigned i = 0; i < list.size(); i++)
            list[i]->MouseMove(mouse);
        return false;
    }
    else
    {
        list[stealFocus]->MouseMove(mouse);
        return true;
    }
}

bool InterfaceManager::TextEntered(char text)
{
    if(currentFocus != -1)
        return list[currentFocus]->TextEntered(text);
    return false;
}

bool InterfaceManager::Input(sf::Keyboard::Key i)
{
    if(currentFocus != -1)
        return list[currentFocus]->Input(i);
    return false;
}

void InterfaceManager::ChangeFocus(GUI *focus)
{
    currentFocus = -1;
    stealFocus = -1;
    if(!focus)
        return;

    for(unsigned i = 0; i < list.size(); i++)
    {
        if(list[i] == focus)
        {
            currentFocus = i;
            list[i]->SetFocus(true);
        }
        else(list[i]->SetFocus(false));
    }
}

void InterfaceManager::Show(GUI *gui)
{
    if(!gui)
        return;

    if(gui->stealFocus)
        for(unsigned i = 0; i < list.size(); i++)
            if(list[i]->stealFocus)
                list[i]->Hide();

    gui->Show();
    ChangeFocus(gui);
}

void InterfaceManager::Hide(GUI *gui)
{
    for(unsigned i = 0; i < list.size(); i++)
        if(!gui || gui == list[i])
            list[i]->Hide();
}

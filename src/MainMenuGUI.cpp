#include "MainMenuGUI.h"

MainMenuGUI::MainMenuGUI(String &title, std::function<void (void)> exitCallback,
                          std::function<void (void)> saveCallback,
                          std::function<void (void)> loadCallback,
                         std::function<void (String)> changeResolutionCallback,
                         sf::Vector2f offset) : GUI(title, offset)
{
    this->exitCallback = exitCallback;
    this->changeResolutionCallback = changeResolutionCallback;
    this->saveCallback = saveCallback;
    this->loadCallback = loadCallback;
    this->size = sf::Vector2f(210, 310);
    this->showOnTop = true;
    this->stealFocus = true;
    this->isFullScreen = true;

    this->proprieties = new Proprieties();

    this->align = ALIGN_CENTER;
    proprieties->list.push_back(new Propriety("Play", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Save", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Load", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Settings", BUTTON, 1,  false, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Full screen", BOOL, 1, true, &isFullScreen));
    proprieties->list.push_back(new Propriety("Exit", BUTTON, 1,  true, NULL, NULL, NULL));

    selected = 4;
}

MainMenuGUI::~MainMenuGUI()
{
    delete proprieties;
}

bool MainMenuGUI::MouseDown(sf::Vector2f mouse)
{
    if(hide)
        return false;

    GUI::MouseDown(mouse);

    if(selected >= (int) proprieties->list.size())
        return false;

    if(selected < 0)
        return false;

    if(selected == 0)
    {
        Hide();
    }
    else if(selected == 1)
    {
        Hide();
        saveCallback();
    }
    else if(selected == 2)
    {
        Hide();
        loadCallback();
    }
    else if(selected == 4)
    {
        changeResolutionCallback(isFullScreen ? "FULLSCREEN" : "DEFAULT");
    }

    else if(selected == 5)
    {
        Hide();
        exitCallback();
    }
    return true;
}

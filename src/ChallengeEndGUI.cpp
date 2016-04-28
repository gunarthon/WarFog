#include "ChallengeEndGUI.h"

ChallengeEndGUI::ChallengeEndGUI(String title, std::function<void (void)> retryCallback,
                            std::function<void (void)> nextCallback,
                            std::function<void (void)> backCallback,
                            sf::Vector2f offset) : GUI(title, offset)
{
    win = false;
    this->retryCallback = retryCallback;
    this->nextCallback = nextCallback;
    this->backCallback = backCallback;

    this->size = sf::Vector2f(210, 110);
    this->proprieties = new Proprieties();

    this->align = ALIGN_DOWN_CENTER;
    proprieties->list.push_back(new Propriety("Try again", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Next", BUTTON, 1,  true, NULL, NULL, NULL));
    proprieties->list.push_back(new Propriety("Back", BUTTON, 1,  true, NULL, NULL, NULL));
}

ChallengeEndGUI::~ChallengeEndGUI()
{
    delete proprieties;
}

bool ChallengeEndGUI::MouseDown(sf::Vector2f mouse)
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
        retryCallback();
    }
    else if(selected == 1)
    {
        Hide();
        nextCallback();
    }
    else if(selected == 2)
    {
        Hide();
        backCallback();
    }
    return true;
}

void ChallengeEndGUI::SetWin(bool win)
{
    title = win ? "CHALLENGE PASSED" : "FAILURE";
}

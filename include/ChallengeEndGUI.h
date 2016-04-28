#ifndef CHALLENGEENDGUI_H
#define CHALLENGEENDGUI_H

#include "GUI.h"


class ChallengeEndGUI : public GUI
{
    public:
        ChallengeEndGUI(String title, std::function<void (void)> retryCallback,
                            std::function<void (void)> nextCallback,
                            std::function<void (void)> backCallback,
                            sf::Vector2f offset = sf::Vector2f(0,10));
        virtual ~ChallengeEndGUI();

        bool MouseDown(sf::Vector2f mouse);
        void SetWin(bool win);
    protected:
        std::function<void (void)> retryCallback;
        std::function<void (void)> nextCallback;
        std::function<void (void)> backCallback;
        bool win;
    private:
};

#endif // CHALENGEENDGUI_H

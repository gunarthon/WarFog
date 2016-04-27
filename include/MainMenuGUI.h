#ifndef MAINMENUGUI_H
#define MAINMENUGUI_H

#include "GUI.h"


class MainMenuGUI : public GUI
{
    public:
        MainMenuGUI(String &title, std::function<void (void)> exitCallback,
                          std::function<void (void)> saveCallback,
                          std::function<void (void)> loadCallback,
                    std::function<void (String)> changeResolutionCallback,
                    sf::Vector2f offset = sf::Vector2f(0,0));
        virtual ~MainMenuGUI();

        bool MouseDown(sf::Vector2f mouse);


        bool isFullScreen;
    protected:
        std::function<void (void)> exitCallback;
        std::function<void (String)> changeResolutionCallback;
      std::function<void (void)> saveCallback;
      std::function<void (void)> loadCallback;

    private:
};

#endif // MAINMENUGUI_H

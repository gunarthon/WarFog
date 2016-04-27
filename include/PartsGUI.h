#ifndef PARTSGUI_H
#define PARTSGUI_H

#include "GUI.h"


class PartsGUI : public GUI
{
    public:
        PartsGUI(String &title, PartList *allParts, sf::Vector2f offset = sf::Vector2f(10,10));
        virtual ~PartsGUI();

        void Draw(sf::RenderWindow *window, sf::Font *font);
        bool MouseDown(sf::Vector2f mouse);
        bool MouseMove(sf::Vector2f mouse);

        bool Input(sf::Keyboard::Key i);

    protected:
        PartList *partList;
    private:
};

#endif // PARTSGUI_H

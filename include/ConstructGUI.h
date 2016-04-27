#ifndef CONSTRUCTGUI_H
#define CONSTRUCTGUI_H

#include "GUI.h"


class ConstructGUI : public GUI
{
    public:
        ConstructGUI();
        virtual ~ConstructGUI();

        virtual void Update();
        virtual void Show();
        virtual void Hide();
        virtual void Draw(sf::RenderWindow *window, sf::Font *font);
        virtual void SetFocus(bool focus = true);
        virtual bool HasFocus();
        virtual bool InsideGUI(sf::Vector2f p);

        virtual bool MouseDown(sf::Vector2f mouse);
        virtual void MouseUp(sf::Vector2f mouse);
        virtual void MouseMove(sf::Vector2f mouse);
        virtual void TextEntered(char text);

        virtual void Input(sf::Keyboard::Key i);
    protected:

    private:
};

#endif // CONSTRUCTGUI_H

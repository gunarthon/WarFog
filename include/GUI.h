#ifndef GUI_H
#define GUI_H

#include "Utils.h"
#include "Proprieties.h"
#include "PartList.h"

class GUI;
typedef std::vector<GUI*> GuiList;

enum Align
{
    ALIGN_NONE,
    ALIGN_CENTER,
    ALIGN_UP_LEFT,
    ALIGN_UP_RIGHT,
    ALIGN_DOWN_RIGHT,
    ALIGN_DOWN_LEFT
};
class GUI
{
    public:
        GUI(String &title, sf::Vector2f offset = sf::Vector2f(10,10));
        virtual ~GUI();
        virtual void Update();
        virtual void Show();
        virtual void Hide();
        virtual void Draw(sf::RenderWindow *window, sf::Font *font);
        virtual void SetFocus(bool focus = true);
        virtual bool HasFocus();
        virtual bool InsideGUI(sf::Vector2f p);

        virtual bool MouseDown(sf::Vector2f mouse);
        virtual bool MouseUp(sf::Vector2f mouse);
        virtual bool MouseMove(sf::Vector2f mouse);
        virtual bool TextEntered(char text);

        virtual bool Input(sf::Keyboard::Key i);

        bool stealFocus;
    protected:
        String title;
        sf::Vector2f pos;
        sf::Vector2f offset;
        sf::Vector2f size;
        sf::Vector2f lastMousePos;
        bool hide;
        bool mouseDown;
        bool focus;
        bool showOnTop;
        int selected;
        sf::Vector2f mouseDownPos;

        Proprieties *proprieties;

        sf::Vector2f identation;
        sf::Vector2f spacing;
        sf::Vector2f scdCollumPos;
        sf::Vector2f trdCollumPos;
        Utils utils;

        Align align;

        Num prevValue;

        virtual void SetProprieties(Proprieties *p);
        virtual int GetPropIndex(sf::Vector2f mouse);

        bool ItemSelected();
    private:
};

#endif // GUI_H

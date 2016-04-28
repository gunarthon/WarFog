#include "GUI.h"
#include "Part.h"

GUI::GUI(String &title, sf::Vector2f offset)
{
    this->title = title;
    this->offset = offset;
    this->pos = sf::Vector2f(0,0);
    this->hide = true;
    this->showOnTop = false;
    this->focus = false;
    this->stealFocus = false;

    this->align = ALIGN_NONE;
    this->size = sf::Vector2f(210, 400);
    this->mouseDown = false;
    this->lastMousePos = sf::Vector2f(9999999,9999999);
    spacing = sf::Vector2f(0, 25);
    identation = sf::Vector2f(10, 5);
    scdCollumPos = sf::Vector2f(0.45f * (size.x-identation.x), 0) + identation;
    trdCollumPos = sf::Vector2f(0.7f * (size.x-identation.x), 0) + identation;
}

GUI::~GUI()
{
    //dtor
}

void GUI::SetProprieties(Proprieties *p)
{
    this->proprieties = p;
}

void GUI::Update()
{

}

void GUI::Show()
{
    hide = false;
    if(showOnTop)
        focus = true;
    else
        focus = false;
    mouseDown = false;
}

void GUI::Hide()
{
    hide = true;
    focus = false;
}

bool GUI::InsideGUI(sf::Vector2f p)
{
    if(hide)
        return false;
    return !(p.x < pos.x || p.y < pos.y ||
       p.x > pos.x + size.x || p.y > pos.y + size.y);
}

void GUI::Draw(sf::RenderWindow *window, sf::Font *font)
{
    if(hide || !proprieties)
        return;

    sf::View view = window->getDefaultView();
    view.setCenter(window->getSize().x/2, window->getSize().y/2);
    view.setSize(window->getSize().x, window->getSize().y);
    window->setView(view);

    sf::RectangleShape rectangle;
    rectangle.setSize(size);
    rectangle.setFillColor(sf::Color(0,0,0,100));
    rectangle.setOutlineThickness(focus ? 3.0f : 2.0f);

    if(align == ALIGN_NONE)
        pos = offset;
    else if(align == ALIGN_CENTER)
        pos = sf::Vector2f(view.getSize().x/2 - rectangle.getLocalBounds().width * 0.5f + offset.x,
            view.getSize().y/2 - rectangle.getLocalBounds().height * 0.5f + offset.y);
    else if(align == ALIGN_UP_RIGHT)
        pos = sf::Vector2f(view.getSize().x - rectangle.getLocalBounds().width - offset.x, offset.y);
    else if(align == ALIGN_DOWN_RIGHT)
        pos = sf::Vector2f(view.getSize().x - rectangle.getLocalBounds().width - offset.x, view.getSize().y - rectangle.getLocalBounds().height - offset.y);
    else if(align == ALIGN_DOWN_LEFT)
        pos = sf::Vector2f(offset.x, view.getSize().y - rectangle.getLocalBounds().height - offset.y);
    else if(align == ALIGN_DOWN_CENTER)
        pos = sf::Vector2f(view.getSize().x/2 - rectangle.getLocalBounds().width/2 + offset.x, view.getSize().y - rectangle.getLocalBounds().height - offset.y);

    rectangle.setPosition(pos);
    window->draw(rectangle);

    sf::Text text;
    text.setFont(*font);
    text.setCharacterSize(14);
    text.setColor(sf::Color::White);

    sf::RectangleShape selRect;
    selRect.setSize(sf::Vector2f(size.x, 18.0f));
    selRect.setFillColor(sf::Color(66,200,255,150));
    selRect.setOutlineThickness(0);

    sf::RectangleShape button;
    button.setSize(sf::Vector2f(size.x - 2*identation.x, 18.0f));
    button.setFillColor(sf::Color::Transparent);
    button.setOutlineThickness(1);
    button.setOutlineColor(sf::Color::White);

    sf::RectangleShape track;
    track.setSize(sf::Vector2f(size.x - trdCollumPos.x - 1.2*identation.x, 16.0f));
    track.setFillColor(sf::Color::White);
    track.setOutlineThickness(0);


    sf::RectangleShape slider;
    slider.setSize(sf::Vector2f(10.0f, 16.0f));
    slider.setFillColor(sf::Color::Black);
    slider.setOutlineColor(sf::Color::Blue);
    slider.setOutlineThickness(-2);


    text.setString(title);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(16);
    text.setPosition(pos.x + size.x/2 - text.getLocalBounds().width/2, pos.y + 4);
    window->draw(text);

    text.setStyle(sf::Text::Regular);
    text.setCharacterSize(14);

    sf::Vector2f yi = pos + spacing;
    for(unsigned i = 0; i < proprieties->list.size(); i++)
    {
        if(!proprieties->list[i]->editable)
            continue;

        Propriety *p = proprieties->list.at(i);
        if(i == selected && p->type != BUTTON && focus)
        {
            selRect.setPosition(yi + sf::Vector2f(0,4));
            window->draw(selRect);
        }

        if(p->type == BUTTON)
        {
            button.setFillColor(sf::Color::Transparent);
            if(i == GetPropIndex(mouseDownPos) && mouseDown && focus)
            {
                sf::Color color = selRect.getFillColor();
                button.setFillColor(color);
            }
            if(i == GetPropIndex(lastMousePos) && !mouseDown)
            {
                sf::Color color = selRect.getFillColor();
                color.a *= 0.5f;
                button.setFillColor(color);
            }
            button.setPosition(yi+identation);
            window->draw(button);
        }

        if(true)
        {
            text.setString(proprieties->list[i]->name.c_str());
            if(p->type == BUTTON)
                text.setPosition(yi.x + size.x/2 - text.getLocalBounds().width/2, yi.y + 4);
            else
                text.setPosition(yi + identation);
            window->draw(text);
        }
        if(p->type != INPUT && p->type != BUTTON)
        {
            text.setString(proprieties->list[i]->ValueToString().c_str());
            text.setPosition(yi + scdCollumPos);
            window->draw(text);
        }
        if(p->type == SLIDER)
        {
            track.setPosition(yi + trdCollumPos);
            window->draw(track);

            slider.setPosition(track.getPosition() - sf::Vector2f(slider.getSize().x / 2, 0)+ sf::Vector2f((*(p->value)  - p->minValue) * track.getSize().x / (p->maxValue - p->minValue), 0));
            window->draw(slider);
        }
        else if(p->type == INPUT)
        {
            if(i == selected && focus)
                text.setString("Press a key...");
            else
                text.setString(utils.InputToString(p->input));
            text.setPosition(yi + scdCollumPos);
            window->draw(text);
        }
        else if(p->type == STRING)
        {
            if(i == selected)
                text.setString(*p->text + "_");
            else
                text.setString(*p->text);
            text.setPosition(yi + scdCollumPos);
            window->draw(text);
        }

        yi += spacing;
    }
}

bool GUI::MouseDown(sf::Vector2f mouse)
{
    focus = false;
    selected = GetPropIndex(mouse);
    if(hide || !proprieties)
        return false;
    if(mouse.x < pos.x || mouse.y < pos.y ||
       mouse.x > pos.x + size.x || mouse.y > pos.y + size.y)
        return false;


    if(selected >= (int) proprieties->list.size())
    {
        selected = -1;
        return false;
    }

    if(selected < 0)
        return false;

    focus = true;
    Propriety *p = proprieties->list.at(selected);
    String newValue;
    if(p->type == BOOL)
    {
        if(p->ValueToString() == "0")
            newValue = "1";
        else
            newValue = "0";
        p->Set(newValue);
    }

    else if(p->type == SLIDER)
    {
        prevValue = *(p->value);
    }
    else if(p->type == BUTTON)
    {

    }

    mouseDown = true;
    mouseDownPos = mouse;

    return true;
}

bool GUI::MouseMove(sf::Vector2f mouse)
{
    lastMousePos = mouse;
    if(hide || !proprieties)
        return false;


    if(selected >= (int) proprieties->list.size() || selected < 0)
        return false;

    if(mouseDown)
    {
        Propriety *p = proprieties->list.at(selected);
        if(p->type == SLIDER)
        {
            String newValue;
            Num xDrag = mouse.x - mouseDownPos.x;
            Num value = prevValue;

            value += xDrag * (p->maxValue - p->minValue) / 200;
            if(value > p->maxValue)
                value = p->maxValue;
            if(value < p->minValue)
                value = p->minValue;

            newValue = utils.ToString(value);
            p->Set(newValue);
        }
    }
    return false;
}

bool GUI::MouseUp(sf::Vector2f mouse)
{
    mouseDown = false;
    return false;
}

bool GUI::TextEntered(char text)
{
    if(hide || !proprieties)
        return false;

    if(selected >= (int) proprieties->list.size() || selected < 0)
        return false;

    Propriety *p = proprieties->list.at(selected);
    if(p->type == STRING)
    {
        if(text == '\r')
            selected = -1;
        else
        {
            String newValue;
            newValue = p->ValueToString();
            if(text == '\b')
            {
                if(newValue.length())
                    newValue.pop_back();
            }
            else
                newValue += text;
            p->Set(newValue);
        }
        return true;
    }
    else if(p->type == POINTER)
    {
        if(text == '\r')
            selected = -1;
        else
        {
            String newValue;
            newValue = p->ValueToString();
            if(text == '\b')
            {
                if(newValue.length())
                    newValue.pop_back();
            }
            else
                newValue += text;
            p->Set(newValue);
        }
        return true;
    }
    return false;
}

void GUI::SetFocus(bool focus)
{
    this->focus = focus;
}

bool GUI::HasFocus()
{
    if(stealFocus)
        return !hide;
    return focus;
}

bool GUI::Input(sf::Keyboard::Key i)
{
    if(hide || !proprieties)
        return false;

    if(i == sf::Keyboard::Escape)
    {
        Hide();
        return false;
    }

    if(selected >= (int) proprieties->list.size() || selected < 0)
        return false;

    Propriety *p = proprieties->list.at(selected);
    if(p->type == INPUT)
    {
        String newValue = utils.InputToString(&i);
        p->Set(newValue);
        selected = -1;
        return true;
    }
    else if(p->type == POINTER)
    {
        return true;
    }
    else if(p->type == STRING)
    {
        return true;
    }
    return false;
}

int GUI::GetPropIndex(sf::Vector2f mouse)
{
    Num y = spacing.y;
    if(hide || !proprieties)
        return -1;
    if(mouse.x < pos.x || mouse.x > pos.x + size.x)
        return -1;
    for(unsigned i = 0; i < proprieties->list.size(); i++)
    {
        if(proprieties->list[i]->editable)
        {
            if(y + spacing.y >= (mouse.y - pos.y) && y < (mouse.y - pos.y))
            {
                return i;
            }
            y += spacing.y;
        }
    }
    return -1;
}
bool GUI::ItemSelected()
{
    return selected != -1;
}

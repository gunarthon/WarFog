#include "ConstructGUI.h"

ConstructGUI::ConstructGUI()
{
    //ctor
}

ConstructGUI::~ConstructGUI()
{
    //dtor
}

bool ConstructGUI::MouseDown(sf::Vector2f mouse)
{
    mouseClicks.push_back(camera->ToWorldPos(sf::Vector2f(x, y)));

    if(currAction == DEFAULT_ACTION)
    {
        for(unsigned i = 0; i < guiList.size(); ++i)
        {
            if(guiList[i]->MouseDown(sf::Vector2f(x,y)))
                return;
        }
        GUI *current = CurrentFocus();
        if(!current)
        {
            if(FindCandidateParts(mouseClicks.back(), false) >= 1)
            {
                bool dragging = false;
                for(int i = allParts.selected.size()-1; i >= 0; --i)
                {
                    if(allParts.selected[i] == allParts.candidate[0])
                    {
                        if(input->KeyDown(sf::Keyboard::LShift))
                            allParts.selected.erase(allParts.selected.begin()+i);

                        dragging = true;
                        break;
                    }
                }
                if(!dragging)
                {
                    if(!input->KeyDown(sf::Keyboard::LShift))
                        allParts.selected.clear();
                    allParts.selected.push_back(allParts.candidate[0]);
                }

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                ChangeAction(BOX_SELECTING);
                mouseClicks.push_back(camera->ToWorldPos(sf::Vector2f(x, y)));
            }
        }

    }
    if(currAction == BOX_SELECTING)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
    }
    else if(currAction == NEW_RECT)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
        else if(actionStep == 1)
        {
            if(mouseClicks.size() >= 2)
            {
                Rectangle *rect = new Rectangle(mouseClicks[0].x, mouseClicks[0].y,
                                        mouseClicks[1].x - mouseClicks[0].x, mouseClicks[1].y - mouseClicks[0].y);

                allParts.selected.clear();
                allParts.Add(rect, &allParts.selected);

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                mouseClicks.pop_back();
                actionStep = 1;
            }
        }
    }
    else if(currAction == NEW_CIRCLE)
    {
        if(actionStep == 0)
        {
            UpdateTempParts();
            actionStep++;
        }
        else if(actionStep == 1)
        {
            if(mouseClicks.size() >= 2)
            {
                Circle *cir = new Circle(sf::Vector2f(mouseClicks[0].x, mouseClicks[0].y),
                                        utils.GetLength(sf::Vector2f(mouseClicks[1] - mouseClicks[0])));

                allParts.selected.clear();
                allParts.Add(cir, &allParts.selected);

                ChangeAction(DEFAULT_ACTION);
            }
            else
            {
                mouseClicks.pop_back();
                actionStep = 1;
            }
        }
    }
    else if(currAction == NEW_FIXED_JOINT)
    {
        if(FindCandidateParts(FindSnapPoint(mouseClicks[0]), true) >= 2)
        {
            FixedJoint *f = new FixedJoint((ShapePart*) allParts.At(allParts.candidate[0]),
                                        (ShapePart*) allParts.At(allParts.candidate[1]),
                                           FindSnapPoint(mouseClicks[0]));

            allParts.selected.clear();
            allParts.Add(f, &allParts.selected);

            ChangeAction(DEFAULT_ACTION);
        }
        else
            ChangeAction(NEW_FIXED_JOINT);
    }
    else if(currAction == NEW_REVOLUTE_JOINT)
    {
        if(FindCandidateParts(FindSnapPoint(mouseClicks[0]), true) >= 2)
        {
            RevoluteJoint *r = new RevoluteJoint((ShapePart*) allParts.At(allParts.candidate[0]),
                                                 (ShapePart*) allParts.At(allParts.candidate[1]),
                                                 FindSnapPoint(mouseClicks[0]));
            allParts.selected.clear();
            allParts.Add(r, &allParts.selected);

            ChangeAction(DEFAULT_ACTION);
        }
        else
            ChangeAction(NEW_REVOLUTE_JOINT);
    }
    else if(currAction == NEW_THRUSTER)
    {
        if(FindCandidateParts(FindSnapPoint(mouseClicks[0]), true) >= 1)
        {
            Thruster *t = new Thruster((ShapePart*) allParts.At(allParts.candidate[0]),
                                                 FindSnapPoint(mouseClicks[0]));

            allParts.selected.clear();
            allParts.Add(t, &allParts.selected);

            ChangeAction(DEFAULT_ACTION);
        }
        else
            ChangeAction(NEW_THRUSTER);
    }

    return false;
}

void ConstructGUI::MouseMove(sf::Vector2f mouse)
{
    lastMousePos = mouse;
    if(hide || !proprieties)
        return;


    if(selected >= (int) proprieties->list.size() || selected < 0)
        return;

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
}

void ConstructGUI::MouseUp(sf::Vector2f mouse)
{
    mouseDown = false;
}

void ConstructGUI::TextEntered(char text)
{
    if(hide || !proprieties)
        return;

    if(selected >= (int) proprieties->list.size() || selected < 0)
        return;

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
    }
}

void ConstructGUI::Input(sf::Keyboard::Key i)
{
    if(hide || !proprieties)
        return;

    if(i == sf::Keyboard::Escape)
    {
        Hide();
        return;
    }

    if(selected >= (int) proprieties->list.size() || selected < 0)
        return;

    Propriety *p = proprieties->list.at(selected);
    if(p->type == INPUT)
    {
        String newValue = utils.InputToString(&i);
        p->Set(newValue);
        selected = -1;
    }
}

#include "EndBox.h"

EndBox::EndBox(std::function<void (bool)> endCallback, sf::Vector2f pos, sf::Vector2f size) : LogicBox(pos, size, 1, 1)
{
    win = false;
    this->endCallback = endCallback;

    proprieties.list.push_back(new Propriety("win", BOOL, 1, true, &win));

    type = "EndBox";
}

EndBox::~EndBox()
{

}

void EndBox::CalculateNextOutputs()
{
    if(inputs[0])
    {
        nextOutputs[0] = true;
        endCallback(win);
    }
}

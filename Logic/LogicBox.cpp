#include "LogicBox.h"

LogicBox::LogicBox(sf::Vector2f pos, sf::Vector2f size, unsigned numOfInputs, unsigned numOfOutputs) : Rectangle(pos.x, pos.y, size.x, size.y)
{
    this->numOfInputs = numOfInputs;
    this->numOfOutputs = numOfOutputs;
    hideInSim = true;
    inputs.insert(inputs.end(), numOfInputs, false);
    outputs.insert(outputs.end(), numOfOutputs, false);
    nextOutputs.insert(nextOutputs.end(), numOfOutputs, false);

    density = -5.0;
    this->color = Color(114,255,170,255);

    proprieties.list.push_back(new Propriety("hideInSim", BOOL, 1, true, &hideInSim));

    type = "Buffer";
}

LogicBox::~LogicBox()
{
    inputs.clear();
    outputs.clear();
    nextOutputs.clear();
}


void LogicBox::Init(b2World *world, b2Body *body)
{

}

void LogicBox::UnInit(b2World *world)
{
    for(unsigned i = 0; i < nextOutputs.size(); i++)
        nextOutputs[i] = false;
}

void LogicBox::Update(b2World *world, InputManager *input)
{
    Rectangle::Update(world, input);
    unsigned cableIt = 0;
    for(unsigned i = 0;i < inputs.size(); i++)
    {
        while(cableIt < m_joints.size())
        {
            if(m_joints[cableIt]->type == "Cable")
                break;
            cableIt++;
        }
        if(cableIt < m_joints.size())
        {
            inputs[i] = m_joints[cableIt]->GetOtherPart(this)->GetOutput(0);
        }
        else
            inputs[i] = false;
    }

    if(nextOutputs.size() == outputs.size())
    {
        for(unsigned i = 0; i < nextOutputs.size(); i++)
            outputs[i] = nextOutputs[i];

            CalculateNextOutputs();
    }
}

void LogicBox::CalculateNextOutputs()
{
    for(unsigned i = 0; i < nextOutputs.size(); i++)
        if(i < inputs.size())
            nextOutputs[i] = inputs[i];
        else
            nextOutputs[i] = false;
}

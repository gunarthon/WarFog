#ifndef LOGICBOX_H
#define LOGICBOX_H

#include "Rectangle.h"


class LogicBox : public Rectangle
{
    public:
        LogicBox(sf::Vector2f pos, sf::Vector2f size, unsigned numOfInputs = 0, unsigned numOfOutputs = 1);
        virtual ~LogicBox();

        virtual void Init(b2World *world, b2Body *body = NULL);
		virtual void UnInit(b2World *world);
		virtual void Update(b2World *world, InputManager *input);
		virtual void CalculateNextOutputs();

        unsigned numOfOutputs;
        unsigned numOfInputs;
        bool hideInSim;

    protected:
        std::vector<bool> inputs;
        std::vector<bool> outputs;
        std::vector<bool> nextOutputs;

    private:
};

#endif // LOGICBOX_H

#ifndef ENDBOX_H
#define ENDBOX_H

#include "LogicBox.h"


class EndBox : public LogicBox
{
    public:
        EndBox(std::function<void (bool)> endCallback, sf::Vector2f pos, sf::Vector2f size);
        virtual ~EndBox();

		void CalculateNextOutputs();

    protected:
        bool win;
        std::function<void (bool)> endCallback;
    private:
};

#endif // ENDBOX_H

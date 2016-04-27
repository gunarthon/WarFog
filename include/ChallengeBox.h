#ifndef CHALLENGEBOX_H
#define CHALLENGEBOX_H

#include "Rectangle.h"


class ChallengeBox : public Rectangle
{
    public:
        ChallengeBox(sf::Vector2f pos, sf::Vector2f size);
        virtual ~ChallengeBox();

        void Update(b2World *world, InputManager *input);
        String ToString();

        void UnInit(b2World *world);

        Num partId;
        Num partIndex;
        bool keepIn;
        bool strict;
        bool toWin;
        bool win;
        bool loose;
        bool memory;
    protected:

    private:
};

#endif // CHALLENGEBOX_H

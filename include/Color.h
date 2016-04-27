#ifndef COLOR_H
#define COLOR_H

#include "Utils.h"

class Color
{
    public:
        Color(Num red = 0.0f, Num green = 0.0f, Num blue = 0.0f, Num alpha = 255.0f);
        Color(const Color& other);
        Color& operator=(const Color& other);
        bool operator==(const Color& other);
        Color Darken();
        sf::Color ToSf();

        Num r, g, b, a;
    protected:

    private:
};

#endif // COLOR_H

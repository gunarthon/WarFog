#include "Color.h"

Color::Color(Num red, Num green, Num blue, Num alpha) : r(red) , g(green) , b(blue) , a(alpha)
{
    //ctor
}


Color::Color(const Color& other)
{
    r = other.r;
    g = other.g;
    b = other.b;
    a = other.a;
}

Color& Color::operator=(const Color& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    r = rhs.r;
    g = rhs.g;
    b = rhs.b;
    a = rhs.a;
    //assignment operator
    return *this;
}

bool Color::operator==(const Color& other)
{
    return (r == other.r &&
            g == other.g &&
            b == other.b &&
            a == other.a);
}

sf::Color Color::ToSf()
{
    return sf::Color(r,g,b,a);
}

Color Color::Darken()
{
    Num factor = 0.7f;
    return Color(r * factor, g * factor, b * factor, a);
}

#ifndef PROPRIETY_H
#define PROPRIETY_H

#include "Utils.h"

class Propriety;

enum ProprietyType
{
    NUM = 0,
    BOOL,
    SLIDER,
    STRING,
    POINTER,
    INPUT,
    BUTTON,
    INTWATCH
};

typedef std::vector<Propriety*> ProprietyList;


class Propriety
{
    public:
        Propriety(String name, ProprietyType type, Num defaultValue, bool editable, bool *flag = NULL,
                  Num *value = NULL, String *text = NULL, Num minValue = 0, Num maxValue = 0);
        Propriety(String name, sf::Keyboard::Key defaultKey, bool editable, sf::Keyboard::Key *input);
        Propriety(String name, bool editable, int *intWatch);

        virtual ~Propriety();
        String ValueToString();
        bool Set(String &newValue);

        String name;
        bool editable;
        ProprietyType type;
        bool *flag;
        Num *value;
        String *text;
        int *intWatch;

        Num defaultValue;
        Num maxValue;
        Num minValue;
        sf::Keyboard::Key defaultKey;
        sf::Keyboard::Key *input;

        Utils utils;
    protected:

    private:
};

#endif // PROPRIETY_H

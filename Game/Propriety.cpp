#include "Propriety.h"

Propriety::Propriety(String name, ProprietyType type, Num defaultValue, bool editable,
                  bool *flag, Num *value, String *text, Num minValue,  Num maxValue)
{
    this->name = name;
    this->flag = flag;
    this->value = value;
    this->text = text;
    this->maxValue = maxValue;
    this->minValue = minValue;
    this->type = type;
    this->editable = editable;
    defaultKey = sf::Keyboard::Space;
    input = NULL;
    intWatch = NULL;
}

Propriety::Propriety(String name, sf::Keyboard::Key defaultKey, bool editable, sf::Keyboard::Key *input)
{
    this->name = name;
    this->defaultKey = defaultKey;
    this->editable = editable;
    this->input = input;
    flag = NULL;
    value = NULL;
    text = NULL;
    intWatch = NULL;
    maxValue = 0;
    minValue = 0;
    type = INPUT;
}

Propriety::Propriety(String name, bool editable, int *intWatch)
{
    this->name = name;
    this->editable = editable;
    this->intWatch = intWatch;
    flag = NULL;
    value = NULL;
    text = NULL;
    type = INTWATCH;
}

Propriety::~Propriety()
{
    //dtor
}

String Propriety::ValueToString()
{
    if(type == BOOL && flag)
        return *flag ? "1" : "0";
    else if(type == NUM && value)
        return utils.ToString(*value);
    else if(type == SLIDER && value)
        return utils.ToString(*value);
    else if(type == STRING && text)
        return *text;
    else if(type == POINTER && value)
        return utils.ToString(int(*value));
    else if(type == INPUT && input)
        return utils.InputToString(input);
    else if(type == INTWATCH && intWatch)
        return utils.ToString(*intWatch);
    else
        return "nan";
}


bool Propriety::Set(String &newValue)
{
    if(type == BOOL && flag)
        *flag = newValue == "0" ? false : true;
    else if(type == NUM && value)
        *value = utils.StrToNum(newValue);
    else if(type == STRING && text)
        *text = newValue;
    else if(type == SLIDER && value)
        *value = utils.StrToNum(newValue);
    else if(type == POINTER && value)
        *value = (int) utils.StrToNum(newValue);
    else if(type == INPUT && input)
        *input = utils.StrToInput(newValue);
    return true;
}

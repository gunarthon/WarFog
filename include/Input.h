#ifndef INPUT_H
#define INPUT_H

#include "Utils.h"

enum InputType
{
    KEYBOARD,
    JOYSTICK,
    MOUSE
};

class Input
{
    public:
        Input();
        virtual ~Input();
        Input& operator=(const Input& other);

        unsigned id;
        InputType type;
        Num value;
    protected:


    private:
};

#endif // INPUT_H

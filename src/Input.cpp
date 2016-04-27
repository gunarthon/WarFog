#include "Input.h"

Input::Input()
{
    //ctor
}

Input::~Input()
{
    //dtor
}

Input& Input::operator=(const Input& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

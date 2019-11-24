#ifndef AZINPUT_H
#define AZINPUT_H

#include "InputObject.h"

class azInput
{
    public:
        virtual ~azInput() {}
        virtual void registerKey(const int &keysym) = 0;
        virtual void registerInput(InputObject *iobject) = 0;
};

#endif

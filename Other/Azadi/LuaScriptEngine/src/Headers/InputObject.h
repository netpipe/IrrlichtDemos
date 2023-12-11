#ifndef INPUTOBJECT_H
#define INPUTOBJECT_H

class InputObject
{
    public:
        virtual ~InputObject() {}

        virtual void pushKey(const int &keysym, const char &ch) = 0;
        virtual void pushClick(const char &button) = 0;
        virtual bool checkClicked(const float &mousex, const float &mousey, const float &mousez, const float cam[3]) = 0;
};

#endif

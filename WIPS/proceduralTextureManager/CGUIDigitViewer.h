#ifndef _MY_IRRLICHT_DIGIT_VIEWER_H_
#define _MY_IRRLICHT_DIGIT_VIEWER_H_

// GUIDigitViewer class by CZestmyr
//
// You can pass a pointer to an s32 integer to it and it will show its value

#include <irrlicht.h>

namespace irr{

namespace gui{

class CGUIDigitViewer : public gui::IGUIElement{
    public:
    //Constructor arguments:
    //env - GUIEnvironment to which this Element will belong
    //par - parent of this Element
    //id - id of this Element
    //intPointer - pointer to the value to be shown
    //digitNumber - number of the digits to be shown beginning at the right
    // (the rest will be clipped)
    //prefix and suffix - the files with the digits have to be named somehow,
    // the pattern is [prefix][0-9][suffix], so if you leave the default prefix
    // and suffix, you'll have to create ten textures named "digit0.tga" - "digit9.tga"
    //rectangle - defines the area, that this Element will take on the screen
    CGUIDigitViewer(gui::IGUIEnvironment* env, gui::IGUIElement* par,
    s32 id = -1, s32* intPointer = 0, s32 digitNumber = 3, c8* prefix = "digit",
    c8* suffix = "tga", core::rect<s32> rectangle = core::rect<s32>(0,0,200,200)):
        IGUIElement(EGUIET_ELEMENT, env, par, id, rectangle), dispVar(intPointer),
        digitNum(digitNumber)
        {
        video::IVideoDriver* driver = Environment->getVideoDriver();
        //This loop loads the digits
        for (s32 i = 0; i != 10; i++){
            core::stringc str;
            str = str + prefix;
            //str = str + i;
		str += i;
            str = str + ".";
            str = str + suffix;
            digits[i] = driver->getTexture(str.c_str());
            }
        }
    virtual void draw(){
        video::IVideoDriver* driver = Environment->getVideoDriver();
        if (dispVar == 0) return;    //if the pointer is null, return
        s32 count = *dispVar;    //copy the variable to display
        s32 order = 1;    //helper variable
        for (s32 i = 1; i != digitNum; i++) order *= 10;    //order is now digitNum-1th power of 10 (default 100)
        if (count < 0) count *= (-1);
        count %= (order * 10);    //clip the shown variable value
        s32 offset = 0;    //helper variable for drawing
        s32 digitWidth = digits[0]->getSize().Width;    //calculate the digit width
        //This loop draws the digits, one by one, till the variable order is 0
        do {s32 digit = count / order;
            driver->draw2DImage(digits[digit], core::position2d<s32>(RelativeRect.UpperLeftCorner.X + (offset * digitWidth),
                RelativeRect.UpperLeftCorner.Y));
            offset++;
            count %= order;
            order /= 10;
            } while (order != 0);
        }
    void setVariable(s32* var){
        dispVar = var;
    }
    s32 getVariable(){
        return *dispVar;
        }
    void invalidate(){
        dispVar = 0;
    }
    private:
    s32* dispVar;
    s32 digitNum;
    video::ITexture* digits[10];
};

} // end namespace gui

} // end namespace irr

#endif

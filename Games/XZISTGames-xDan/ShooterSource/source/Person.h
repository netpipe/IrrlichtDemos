
#ifndef __Person_h
#define __Person_h

#include "Target.h"

class Person : public Target
{
public:
    Person(IrrlichtDevice *);

private:
    double stickAge;
    bool slidingDown;
    core::vector3df stickPos;
    video::ITexture *bloodStainTexture;
    bool bigSprayed;
    void secondUpdate(double);
    void hitScore();
    void hitEffects(core::line3df);
    void bigSpray();
};

#endif

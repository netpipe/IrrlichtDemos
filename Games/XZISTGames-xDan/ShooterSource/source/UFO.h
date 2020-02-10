
#ifndef __UFO_h
#define __UFO_h

#include "Target.h"

class UFO : public Target
{
public:
    UFO(IrrlichtDevice *);

private:
    void secondUpdate(double);
    void hitScore();
    void hitEffects(core::line3df);
};

#endif

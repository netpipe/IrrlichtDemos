
#ifndef __Demon_h
#define __Demon_h

#include "Target.h"

class Demon : public Target
{
public:
    Demon(IrrlichtDevice *);

private:
    f32 moveDir;
    void secondUpdate(double);
    void hitScore();
    void hitEffects(core::line3df);
};

#endif

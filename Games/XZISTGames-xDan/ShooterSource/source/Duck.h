
#ifndef __Duck_h
#define __Duck_h

#include "Target.h"

class Duck : public Target
{
public:
    Duck(IrrlichtDevice *);

private:
    f32 moveDir;
    bool shotOnce;
    void secondUpdate(double);
    void hitScore();
    void hitEffects(core::line3df);
};

#endif

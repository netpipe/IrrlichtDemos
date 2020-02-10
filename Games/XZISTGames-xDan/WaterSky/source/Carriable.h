
#ifndef __Carriable_h
#define __Carriable_h

#include "Object.h"

class Carriable : public Object
{
public:
    Carriable(WorldBase *);
    void update(f32);

protected:
    core::stringc pickUpSound;

private:
    bool beingCarried;
};

#endif

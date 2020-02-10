
// An AI controlled creature (enemy?) which moves around


#ifndef __Creature_h
#define __Creature_h

#include "Actor.h"

class Creature : public Actor
{
public:
    Creature(WorldBase *, core::vector3df = core::vector3df(0,0,0));
    
    Object *createNew();
    
    OBJECT_TYPE getType() {return OBJECT_TYPE_CREATURE; }
};

#endif


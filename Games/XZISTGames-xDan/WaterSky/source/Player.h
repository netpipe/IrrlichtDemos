
#ifndef __Player_h
#define __Player_h

#include "Actor.h"

class Player : public Actor
{
public:
    Player(WorldBase *, core::vector3df = core::vector3df(0,0,0));
    
    Object *createNew();
    
    OBJECT_TYPE getType() {return OBJECT_TYPE_PLAYER; }
};

#endif

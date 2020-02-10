
#ifndef __Block_h
#define __Block_h

#include "Object.h"

class Block : public Object
{
public:
    Block(WorldBase *, core::vector3df = core::vector3df(0,0,0));
    
    Object *createNew();
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_BLOCK; }
};

#endif


#ifndef __FuelCan_h
#define __FuelCan_h

#include "Carriable.h"

class FuelCan : public Carriable
{
public:
    FuelCan(WorldBase *, core::vector3df = core::vector3df(0,0,0));
    
    Object *createNew();
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_FUELCAN; }
};

#endif

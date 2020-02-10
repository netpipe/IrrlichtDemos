
#ifndef __Scenery_h
#define __Scenery_h

#include "Object.h"

class Scenery : public Object
{
public:
    Scenery(WorldBase *, const wchar_t * = L"", core::vector3df = core::vector3df(0,0,0),
        core::vector3df = core::vector3df(0,0,0));
    
    Object *createNew();
    
    void setFileName(const wchar_t *);
    void createMesh();
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_SCENERY; }
};

#endif

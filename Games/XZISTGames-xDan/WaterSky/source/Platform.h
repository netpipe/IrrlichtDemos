
#ifndef __Platform_h
#define __Platform_h

#include "Object.h"
/*
class PlatformCreator : ObjectCreator
{
    Object *createNew(WorldBase *world)
    {
        return new Platform(world);    
    }        
};    
*/
class Platform : public Object
{
public:
    Platform(WorldBase *, const wchar_t * = L"", core::vector3df = core::vector3df(0,0,0),
        core::vector3df = core::vector3df(0,0,0));
    
    Object *createNew();
    
    void setFileName(const wchar_t *);
    void createMesh();
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_PLATFORM; }
};

#endif

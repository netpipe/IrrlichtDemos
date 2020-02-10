
#ifndef __Generator_h
#define __Generator_h

#include "Object.h"

class Generator : public Object
{
public:
    Generator(WorldBase *,
        core::vector3df = core::vector3df(0,0,0),
        core::vector3df = core::vector3df(0,0,0)
        );
    
    Object *createNew();
    
    void update(f32);
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_GENERATOR; }
    
private:
    void createSmokeEmitter();
    scene::IParticleSystemSceneNode *psSmoke;
    core::vector3df lastRotation;
};

#endif

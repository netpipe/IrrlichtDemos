
#ifndef __ThirdPersonCamera_h
#define __ThirdPersonCamera_h

#include "WorldBase.h"

class ThirdPersonCamera : public WorldBase
{
public:
    ThirdPersonCamera(WorldBase *);
    
    void setup(scene::ISceneNode *);
    void update(f32);
    
private:
    scene::ISceneNode *playerNode;
    core::vector3df positionOffset;
};

#endif

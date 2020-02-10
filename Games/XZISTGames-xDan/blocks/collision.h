
#ifndef __COLLISION_H
#define __COLLISION_H

#include <ode/ode.h>

enum {COLLISION_BLOCK, COLLISION_MAGNET, COLLISION_PLAYER, COLLISION_SELECTRAY, COLLISION_UNKNOWN};

class Collision
{
public:
    Collision();
    
    int type;
    void *data;
    
    // ODE material
    bool surfaceSet;
    dSurfaceParameters surface;
    
    // unused?
    void add();
    void reset();
    int getAmount();
    void setRayContact(dContactGeom contact);
    dContactGeom getRayContact();
private:
    int number;
    dContactGeom rayContact;
};

#endif

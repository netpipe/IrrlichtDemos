
#ifndef __COLLISION_H
#define __COLLISION_H

#include <ode/ode.h>

class Collision
{
public:
    Collision();
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

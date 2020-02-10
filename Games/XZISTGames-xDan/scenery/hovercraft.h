
#ifndef __HOVERCRAFT_H
#define __HOVERCRAFT_H

#include "spaceship.h"
#include "CBeamNode.h"

class HoverCraft: public SpaceShip
{
public:
    HoverCraft(irrInfo *);
    ~HoverCraft();
    void Update();
    
private:
    dReal HoverHeight;
    
    /*
     * Exhaust jet scene nodes
     */
    CBeamNode *exhaustLeft;
    CBeamNode *exhaustRight;
    
    bool leftThrustEmitting, rightThrustEmitting;
    
    dGeomID dRayGeom;
    Collision GroundRayCollision;
};

#endif

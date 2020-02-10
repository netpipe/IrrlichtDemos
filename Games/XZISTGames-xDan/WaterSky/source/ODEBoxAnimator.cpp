
#include "ODEBoxAnimator.h"

ODEBoxAnimator::ODEBoxAnimator(scene::ISceneNode *node, ODEPhysics *dPhysics, u32 creationFlags, core::vector3df size)
    : ODEAnimator(node, dPhysics, creationFlags)
{
    dReal density = 0.01;
    
    dBodyID dBody = dBodyCreate(dPhysics->dWorld);
    dMass mass;
    dMassSetBox(&mass, density, size.X,size.Y,size.Z);
    dBodySetMass(dBody, &mass);
    
    dGeomID dGeom = dCreateBox(dPhysics->dMainSpace, size.X,size.Y,size.Z);
    dGeomSetBody(dGeom, dBody);
    
    dGeoms.push_back(dGeom);
    dBodies.push_back(dBody);
}





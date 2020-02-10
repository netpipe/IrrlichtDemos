
#include "ODESphereAnimator.h"

ODESphereAnimator::ODESphereAnimator(scene::ISceneNode *node, ODEPhysics *dPhysics, u32 creationFlags, f32 radius)
    : ODEAnimator(node, dPhysics, creationFlags)
{
    dReal density = 0.01;
    
    dBodyID dBody = dBodyCreate(dPhysics->dWorld);
    dMass mass;
    dMassSetSphere(&mass, density, radius);
    dBodySetMass(dBody, &mass);
    
    dGeomID dGeom = dCreateSphere(dPhysics->dMainSpace, radius);
    dGeomSetBody(dGeom, dBody);
    
    dGeoms.push_back(dGeom);
    dBodies.push_back(dBody);
}





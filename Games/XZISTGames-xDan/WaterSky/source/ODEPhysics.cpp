
#include "ODEPhysics.h"
#include "ODEAnimator.h"

ODEPhysics::ODEPhysics()
{
    dInitODE();
    dWorld = dWorldCreate();
    
    dMainSpace = dHashSpaceCreate(0);
    
    dPerStepContactJointGroup = dJointGroupCreate(0);
    
    // setup some values
    dWorldSetGravity(dWorld, 0, -9.8, 0);
	//dWorldSetERP(dWorld, 0.2);
	//dWorldSetContactSurfaceLayer(dWorld, 0.001);
	//dWorldSetCFM(dWorld, 0.00001);
	
	accumulator = 0.0;
}

ODEPhysics::~ODEPhysics()
{
    dSpaceDestroy(dMainSpace);
    dWorldDestroy(dWorld);
    dJointGroupDestroy(dPerStepContactJointGroup);
    dCloseODE();
}

void ODEPhysics::updatePhysics(f32 dt)
{
    const f32 maxTime = 0.1; // lower limit of 10 FPS...
    if (dt > maxTime) dt = maxTime;
    
    const f32 simStep = 0.002;//0.01;
    
    
    /* NOTE
    This sim stepping method is slightly hackish.
    A better solution is documented here: http://www.gaffer.org/game-physics/fix-your-timestep/
    But it requires things like quaternion interpolation which would make my brain explode.
    Another option is to use a physics engine like Bullet which deals with this
    malarkey for you. (I haven't used it myself though)
    */
    
    accumulator += dt;
    
    //int stepCount = 0;
    
    while (accumulator >= simStep)
    {
        // add all forces etc...
        for (u32 i = 0; i < dAnimators.size(); i ++)
        {
            dAnimators[i]->preStep();
        }
        
        dSpaceCollide(dMainSpace, this, dNearCallback);
        dWorldQuickStep(dWorld, simStep);
        dJointGroupEmpty(dPerStepContactJointGroup);
        
        accumulator -= simStep;
        
        //stepCount++;
    }
    
    // clear all accumulated forces etc
    for (u32 i = 0; i < dAnimators.size(); i ++)
    {
        dAnimators[i]->endUpdate();
    }
    
    //printf("Steps: %i dt: %f actual dt: %f\n", stepCount, ((float)stepCount)*simStep, dt);
}

void ODEPhysics::setRotation(core::CMatrix4<f32> source, dMatrix3 dest)
{
    source = source.getTransposed();
    
    for (int i = 0; i < 3; i ++)
    {
        int j = 0;
        for (; j < 3; j ++)
        {
            dest[i*4+j] = source[i*4+j];
        }
        dest[i*4+j+1] = 0;
    }
}

void ODEPhysics::setRotation(const dMatrix3 source, core::CMatrix4<f32> &dest)
{
    dest = dest.getTransposed();
    
    for (int i = 0; i < 3; i ++)
    {
        int j = 0;
        for (; j < 3; j ++)
        {
            dest[i*4+j] = source[i*4+j];
        }
        dest[i*4+j+1] = 0;
    }
    
    dest = dest.getTransposed();
}

void ODEPhysics::dNearCallback(void *data, dGeomID o1, dGeomID o2)
{
    if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
    {
        // NOT SURE ABOUT CODE HERE (e.g. are all three function calls needed? or just first? or just last 2?
        // collide geom with all geoms contained by space
        dSpaceCollide2(o1, o2, data, dNearCallback); 
        // collide all geoms inside this space with each other
        if (dGeomIsSpace(o1)) dSpaceCollide((dSpaceID)o1, data, dNearCallback);
        if (dGeomIsSpace(o2)) dSpaceCollide((dSpaceID)o2, data, dNearCallback);
    }
    else
    {
        // collide two geoms!
        ((ODEPhysics *)data)->geomCollide(o1,o2);
    }
}

void ODEPhysics::geomCollide(dGeomID o1, dGeomID o2)
{
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    
    // don't generate contacts if both are bodiless.
    // this is mainly to prevent intersecting static geoms (e.g. scenery) from hogging resources.
    // however in some cases this would be undesirable (e.g. you have non physically simulated objects
    // that you still want to detect collisions with... Ghosts maybe?)
    if ( !(b1 || b2) ) return;
    
    // If both geoms have bodies, which are joined by a non contact joint, then return.
    // e.g. could be useful for a ragdoll, if limbs overlap. However, ODE might deal with this
    // so it might not be needed?
    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;
    
    // setup an array of available contact points
    const int MAX_CONTACTS = 12;
    dContact contact[MAX_CONTACTS];
    
    int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
    
    if (numc == 0) return;
    
    // perhaps a method of material would be called here e.g. to play a sound etc...
    // material->onContact(contact, numc);
    
    if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass))
    {
        // need code here to report ray collisions
        return;
    }
    
    // for each contact, create an actual contact joint between two bodies
    // these will be removed after simulation stepping
    for (int i = 0; i < numc; i++)
    {
        // setup contact surface variables
        // could have some sort of Material structure pointed to in geom data, and use that here.
        contact[i].surface.mode =  dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = 100;
        contact[i].surface.bounce = 0;
        contact[i].surface.bounce_vel = 0;
        contact[i].surface.soft_cfm = 0;
        
        dJointID c = dJointCreateContact(dWorld, dPerStepContactJointGroup, contact+i);
        dJointAttach(c, b1, b2);
    }
}


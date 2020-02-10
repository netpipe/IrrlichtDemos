
#include "ODEAnimator.h"
#include "ODEPhysics.h"



ODEAnimator::ODEAnimator(scene::ISceneNode *node, ODEPhysics *dPhysics, u32 creationFlags)
{
    this->dPhysics = dPhysics;
    this->creationFlags = creationFlags;
    
    lastTimeMs = 0;
    endUpdate();
    
    dPhysics->dAnimators.push_back(this);
}

ODEAnimator::~ODEAnimator()
{
    for (u32 i = 0; i < dPhysics->dAnimators.size(); i ++)
    {
        if (dPhysics->dAnimators[i] == this)
        {
            dPhysics->dAnimators.erase(i);
            break;
        }
    }
    clearAll();
}

void ODEAnimator::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    if (!node) return;
    
    // on first animate, no delta time
    if (lastTimeMs == 0) lastTimeMs = timeMs;
	//f32 dt = ((f32)(timeMs-lastTimeMs)) / 1000.0;
	lastTimeMs = timeMs;
	
	// if animated, could clear all and then remakeGeomsFromMesh using a frame number
    
    putTransformation(node);
}

void ODEAnimator::takeTransformation(scene::ISceneNode *node)
{
    node->updateAbsolutePosition();
    
    core::CMatrix4<f32> mat = node->getAbsoluteTransformation();
    
	// for positioning etc, should probably use an ode setting function that takes a matrix?
	// for rotation (and scale?)
	
	// set geom's position from node
	// assume all geoms are at same position, as currently only time there will be multiple
	// geoms is with a mesh. Would need to rethink this if we had rag dolls etc...
	core::vector3df pos = mat.getTranslation();
	
	if (creationFlags & ODEANIM_CENTRE_NODE) pos += node->getBoundingBox().getCenter();
	
    for (u16 i = 0; i < dGeoms.size(); i ++)
    {
        dGeomSetPosition(dGeoms[i], pos.X, pos.Y, pos.Z);
    }
    
    dMatrix3 rota;
    dPhysics->setRotation(mat, rota);
    
    for (u16 i = 0; i < dGeoms.size(); i ++)
    {
        dGeomSetRotation(dGeoms[i], rota);
    }
    
    // if scale changed, need to recreate trimesh or modify other geoms
}

void ODEAnimator::putTransformation(scene::ISceneNode *node)
{
    
    // get positions and rotations from bodies
    // NOTE:
    // only time there will be > 1 bodies is when it is a rag doll?
    if (dBodies.size() == 1)
    {
        // to position absolutely, would need to take into account parent's transformation
        // (as it is a node cannot be a transformed child)
        // ODE joints are the only things to be used for hierarchy?
        const dReal *pos = dBodyGetPosition(dBodies[0]);
        core::vector3df newPos;
        newPos.X = pos[0];
        newPos.Y = pos[1];
        newPos.Z = pos[2];
        
        if (creationFlags & ODEANIM_CENTRE_NODE) newPos -= node->getBoundingBox().getCenter();
        
        node->setPosition(newPos);
        
        // set rotation
        // note: we have the same conerns as above with position
        
        if ( !(creationFlags & ODEANIM_NO_ROTATION)
            && !(creationFlags & ODEANIM_NO_ROTATIONXZ) )
        {
            core::CMatrix4<f32> mat;
            dPhysics->setRotation(dBodyGetRotation(dBodies[0]), mat);
            node->setRotation(mat.getRotationDegrees());
        }
        
        if (creationFlags & ODEANIM_NO_ROTATIONXZ)
        {
            // Just set Y rotation
            core::CMatrix4<f32> mat;
            dPhysics->setRotation(dBodyGetRotation(dBodies[0]), mat);
            core::vector3df currentRot = node->getRotation();
            core::vector3df newRot = mat.getRotationDegrees();
            newRot.X = currentRot.X;
            newRot.Z = currentRot.Z;
            node->setRotation(newRot);
        }
    }
    node->updateAbsolutePosition();
}

void ODEAnimator::endUpdate()
{
    core::vector3df none(0,0,0);
    force = none;
    torque = none;
    torqueDamping = 0;
    
    linearVelSet = false;
    angularVelSet = false;
}

void ODEAnimator::addForce(core::vector3df more)
{
    force += more;
}

void ODEAnimator::addTorque(core::vector3df more)
{
    torque += more;
}

void ODEAnimator::addTorqueDamping(f32 more)
{
    torqueDamping += more;
}

void ODEAnimator::setLinearVel(core::vector3df vel)
{
    for (unsigned int i = 0; i < dBodies.size(); i ++)
    {
        dBodySetLinearVel( dBodies[i], vel.X,vel.Y,vel.Z);
    }
}

void ODEAnimator::setAngularVel(core::vector3df vel)
{
    for (unsigned int i = 0; i < dBodies.size(); i ++)
    {
        dBodySetAngularVel( dBodies[i], vel.X,vel.Y,vel.Z);
    }
}

void ODEAnimator::maintainLinearVel(core::vector3df vel)
{
    linearVel = vel;
    linearVelSet = true;
}

void ODEAnimator::maintainAngularVel(core::vector3df vel)
{
    angularVel = vel;
    angularVelSet = true;
}

void ODEAnimator::preStep()
{
    // add forces etc to all bodies
    
    for (unsigned int i = 0; i < dBodies.size(); i ++)
    {
        dBodyAddForce( dBodies[i], force.X,force.Y,force.Z);
        dBodyAddTorque( dBodies[i], torque.X,torque.Y,torque.Z);
        
        const dReal *angVel = dBodyGetAngularVel(dBodies[i]);
        dBodyAddTorque(dBodies[i], angVel[0]*-torqueDamping, angVel[1]*-torqueDamping, angVel[2]*-torqueDamping);
        
        if (linearVelSet) dBodySetLinearVel( dBodies[i], linearVel.X,linearVel.Y,linearVel.Z);
        if (angularVelSet) dBodySetAngularVel( dBodies[i], angularVel.X,angularVel.Y,angularVel.Z);
    }
}

void ODEAnimator::clearAll()
{
    for (u16 i = 0; i < dGeoms.size(); i ++)
    {
        dGeomDestroy(dGeoms[i]);
    }
    for (u16 i = 0; i < dBodies.size(); i ++)
    {
        dBodyDestroy(dBodies[i]);
    }
}



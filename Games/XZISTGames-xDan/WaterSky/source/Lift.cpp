
#include "Lift.h"
#include "ODEMeshAnimator.h"
#include "ODEBoxAnimator.h"

Lift::Lift(WorldBase *world, LocRot start, LocRot end)
    : Object(world)
{
    this->sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("lift.b3d"));
    
    // physics animator
    dAnimator = new ODEBoxAnimator(sceneNode, dPhysics, ODEANIM_CENTRE_NODE,
        sceneNode->getTransformedBoundingBox().getExtent());
    sceneNode->addAnimator(dAnimator);
    dAnimator->drop();
    
    reservePoints(2);
    setStartPoint(start);
    setEndPoint(end);
    
    createSoundAnimator();
}

Object *Lift::createNew()
{
    return new Lift(this);
}

void Lift::setStartPoint(LocRot point)
{
    points[START] = point;
    
    moving = false;
    goingStartToEnd = true;
    actorGotOff = true;
    
    setPosition(points[START].pos);
    setRotation(points[START].rot);
}

void Lift::setEndPoint(LocRot point)
{
    points[END] = point;
}

void Lift::update(f32 dt)
{
    core::vector3df centre = points[START].pos + (points[END].pos - points[START].pos) / 2.0;
    
    LocRot start,end;
    
    if (goingStartToEnd)
    {
        start = points[START];
        end = points[END];
    }
    else
    {
        start = points[END];
        end = points[START];
    }
    
    if (moving && active)
    {
        f32 startToEndDist = (end.pos - start.pos).getLength();
    
        // vel is towards current destination
        core::vector3df vel = (end.pos - getPosition()).normalize()*speed;
    
        // rvel is towards desired rotation
        core::vector3df rvel;
    
        // fraction of journey covered
        f32 travelled = getPosition().getDistanceFrom(start.pos) / startToEndDist;
        if (travelled > 1.0) travelled = 1.0;
    
        /*
        // There were some problems with quaternions... But anyway interpolating as vectors seems to work fine!
        core::quaternion qstart(start.rot);
        core::quaternion qend(end.rot);
        core::quaternion qdesired;
        qdesired.slerp(qstart, qend, travelled);
        qdesired.toEuler(rvel);
    
        rvel *= core::RADTODEG;
        */
    
        rvel = end.rot.getInterpolated(start.rot, travelled);
    
        // Set rotation, getting ODE position first
        dAnimator->putTransformation(sceneNode);
        setRotation(rvel);
        
        
        // set ode velicity
        dAnimator->maintainLinearVel(vel);
        dAnimator->maintainAngularVel(core::vector3df(0,0,0));
        
        // arrived when
        if (
            // closer to destination than start
            getPosition().getDistanceFrom(end.pos) < getPosition().getDistanceFrom(start.pos)
            &&
            // and distance to centre is greater than half the distance from start to end
            getPosition().getDistanceFrom(centre) >= startToEndDist/2.0
            )
        {
            moving = false;
            goingStartToEnd ^= 1; // switch direction
            soundAnimator->play("media/sfx/hithard.wav", SOUNDANIM_ENQUEUE);
        }
    }
    else
    {
        dAnimator->maintainLinearVel(core::vector3df(0,0,0));
        dAnimator->maintainAngularVel(core::vector3df(0,0,0));
        
        // keep it still and upright
        sceneNode->setPosition(start.pos);
        sceneNode->setRotation(start.rot);
        dAnimator->takeTransformation(sceneNode);
        
        if (active) // IF ACTIVE
        {
        
        // check distance to nearest actor
        Object *obj = getNearestObject(OBJECT_TYPE_PLAYER);
        
        if (obj)
        {
            if (getBoundsDistance(obj) < -2.4)
            {
                if (actorGotOff)
                {
                    moving = true;
                    actorGotOff = false;
                }
            }
            else // don't let lift abandon an actor - move lift to pick it up
            {
                actorGotOff = true;
                
                Object *startObj = getNearestObject(points[START].pos, OBJECT_TYPE_PLAYER);
                Object *endObj = getNearestObject(points[END].pos, OBJECT_TYPE_PLAYER);
                
                // Chose object which is closest to start/end pos. Was start pos object chosen?
                bool chosenObjIsAtStartPos = (startObj->getNode()->getPosition().getDistanceFrom(points[START].pos) < 
                    endObj->getNode()->getPosition().getDistanceFrom(points[END].pos))
                    ?
                    true : false;
                
                if (chosenObjIsAtStartPos)
                {
                    if (sceneNode->getPosition().getDistanceFrom(points[END].pos)
                        < sceneNode->getPosition().getDistanceFrom(points[START].pos))
                        // lift is in wrong (end) position
                    {
                        // Going to pick up Actor at startPos
                        goingStartToEnd = false;
                        moving = true;
                    }
                }
                else // object at end position
                {
                    if (sceneNode->getPosition().getDistanceFrom(points[START].pos)
                        < sceneNode->getPosition().getDistanceFrom(points[END].pos))
                        // ...but lift at start position
                    {
                        // Going to pick up Actor at endPos
                        goingStartToEnd = true;
                        moving = true;
                    }
                }
            }
        }
        if (moving) // wasn't before...
        {
            soundAnimator->play("media/sfx/hithard.wav", SOUNDANIM_ENQUEUE|SOUNDANIM_NO_WAIT);
            soundAnimator->play("media/sfx/liftrun.wav", SOUNDANIM_ENQUEUE|SOUNDANIM_LOOP|SOUNDANIM_UNTIL_NEXT);
        }
        } // END IF ACTIVE
    }
    /*
    // keep upright
    //sceneNode->setRotation(points[START].rot);
    // for all ODE bodies (probably only one...)
    core::array <dBodyID> *dBodies = dAnimator->getBodies();
    for (unsigned int i = 0; i < dBodies->size(); i ++)
    {
        // zero angular velocity
        //dBodySetAngularVel( (*dBodies)[i], 0,0,0);
        // tie X/Z position
        //const dReal *pos = dBodyGetPosition((*dBodies)[i]);
        //sceneNode->setPosition(core::vector3df(points[START].pos.X,pos[1],points[START].pos.Z));
        dAnimator->takeTransformation(sceneNode);
    }
    */
}



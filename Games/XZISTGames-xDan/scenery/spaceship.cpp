
#include "spaceship.h"

SpaceShip::SpaceShip(irrInfo *info) : Entity(info)
{
    // clear movement flags
    clearGoingFlags();
    crashing = false;
    dBody = dBodyCreate(dWorld);
}

SpaceShip::~SpaceShip()
{
    if (dBody) dBodyDestroy(dBody);
    if (dGeom) dGeomDestroy(dGeom);
    irrNode->removeAll();
    irrNode->remove();
}

void SpaceShip::Refresh()
{
    Update();
    irrNode->updateAbsolutePosition();
    // reset collision structure
    collision.reset();
    // reset movement flags
    clearGoingFlags();
}

void SpaceShip::setPosition(dReal x, dReal y, dReal z)
{
    dGeomSetPosition(dGeom, x, y, z);
}

dReal *SpaceShip::getPosition()
{
    return (dReal *)dGeomGetPosition(dGeom);
}

vector3df SpaceShip::getRotation()
{
    dQuaternion result;
    dGeomGetQuaternion(dGeom, result);
    vector3df rot;
    QuaternionToEuler(result, rot);
    return rot;
}

const dReal *SpaceShip::getRotationMatrix()
{
    return dGeomGetRotation(dGeom);
}

void SpaceShip::goForwards()
{
    goingForwards = true;
}

void SpaceShip::goLeft()
{
    goingLeft = true;
}

void SpaceShip::goRight()
{
    goingRight = true;
}

void SpaceShip::clearGoingFlags()
{
    goingForwards = false;
    goingLeft = false;
    goingRight = false;
}


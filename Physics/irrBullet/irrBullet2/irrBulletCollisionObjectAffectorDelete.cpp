// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include "irrBulletCollisionObjectAffectorDelete.h"
#include "irrBulletWorld.h"
#include "irrBulletCollisionObject.h"
#include <vector3d.h>

ICollisionObjectAffectorDelete::ICollisionObjectAffectorDelete(irr::u32 delay)
{
    EndTime = 0;
    TimeDelay = delay;
    FirstRun = true;
	Type = ECollisionObjectAffectorType::ECOAT_DELETE_AFFECTOR;
}

void ICollisionObjectAffectorDelete::affectObject(ICollisionObject* object, irr::u32 timeMS)
{
    if(FirstRun == true)
    {
        EndTime = timeMS + TimeDelay;
        FirstRun = false;
    }

    else
    if(timeMS >= EndTime)
    {
        HasFinished = true;

        object->getDynamicsWorld()->addToDeletionQueue(object);
    }
}

ICollisionObjectAffectorDelete::~ICollisionObjectAffectorDelete()
{
    //dtor
}

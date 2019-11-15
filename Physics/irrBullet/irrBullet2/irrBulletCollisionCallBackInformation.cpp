// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include "irrBulletCollisionCallBackInformation.h"
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include "irrBulletCollisionObject.h"
#include "irrBulletWorld.h"

ICollisionCallbackInformation::ICollisionCallbackInformation(btPersistentManifold* const manifold, irrBulletWorld* world)
{
    contactManifold = manifold;
    dynamicsWorld = world;
}

ICollisionObject* const ICollisionCallbackInformation::getBody0() const
{
    SCollisionObjectIdentification* identification =
		static_cast<SCollisionObjectIdentification*>((contactManifold->getBody1())->getUserPointer());

    return identification->getCollisionObject();

    return 0;
}

ICollisionObject* const ICollisionCallbackInformation::getBody1() const
{
    SCollisionObjectIdentification* identification =
		static_cast<SCollisionObjectIdentification*>((contactManifold->getBody1())->getUserPointer());

    return identification->getCollisionObject();

    return 0;
}


SManifoldPoint& ICollisionCallbackInformation::getContactPoint(irr::u32 index)
{
    btManifoldPoint &point = contactManifold->getContactPoint(index);
    manifoldPoint.setInfo(point);
    return manifoldPoint;
}

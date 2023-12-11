// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include <iostream>
#include "irrBulletCollisionObjectAffector.h"
#include "irrBulletCompileConfig.h"

ICollisionObjectAffector::ICollisionObjectAffector()
{
    #ifdef IRRBULLET_DEBUG_MODE
        printf("irrBullet: Creating object affector\n");
    #endif

    DebugDraw = false;
    HasFinished = false;
}

ICollisionObjectAffector::~ICollisionObjectAffector()
{
    #ifdef IRRBULLET_DEBUG_MODE
        printf("irrBullet: Deleting object affector\n");
    #endif
}

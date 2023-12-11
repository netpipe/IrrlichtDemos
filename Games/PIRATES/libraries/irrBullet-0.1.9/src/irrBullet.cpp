// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include "irrBullet.h"


irrBulletWorld* createIrrBulletWorld(irr::IrrlichtDevice* const device, bool useGImpact, bool useDebugDrawer)
{
    auto world = new irrBulletWorld(device, useGImpact, useDebugDrawer);

    return world;
}

irrBulletWorld *createIrrBulletWorld(std::shared_ptr<irr::IrrlichtDevice> device, bool useGImpact, bool useDebugDrawer)
{
	auto world = new irrBulletWorld(device, useGImpact, useDebugDrawer);

	return world;
}

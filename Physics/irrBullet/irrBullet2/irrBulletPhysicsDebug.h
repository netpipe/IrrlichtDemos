// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

// This code belongs to RandomMesh from the Irrlicht forum

#pragma once

#include <LinearMath/btIDebugDraw.h>
#include "irrBulletCommon.h"
#include <memory>

namespace irr
{
	class IrrlichtDevice;
	class ILogger;

	namespace video
	{
		class IVideoDriver;
	}
}


/// This should be left for internal use by irrBullet.
/*!
    If you set the boolean parameter for irrBulletWorld::debugDrawWorld() to false,
    then you must remember to call these two lines:
        IVideoDriver::setMaterial(material);
        IVideoDriver::setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
    before calling irrBulletWorld::debugDrawWorld();
*/
class IPhysicsDebugDraw : public btIDebugDraw
{

public:
	IPhysicsDebugDraw(std::shared_ptr<irr::IrrlichtDevice> device);

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	void reportErrorWarning(const char* text);

	void draw3dText(const btVector3& location, const char* text);

	void setDebugMode(int mode) { this->mode = static_cast<irrPhysicsDebugMode>(mode); }

	int getDebugMode() const { return static_cast<int>(this->mode); }

private:
	irrPhysicsDebugMode mode;

	irr::video::IVideoDriver* const driver;

	irr::ILogger* logger;
};

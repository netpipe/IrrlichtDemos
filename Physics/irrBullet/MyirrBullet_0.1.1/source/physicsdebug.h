#ifndef __PHYSICS_DEBUG_H_INCLUDED__
#define __PHYSICS_DEBUG_H_INCLUDED__

#include "LinearMath/btIDebugDraw.h"
#include "common.h"

namespace irr
{
	class IrrlichtDevice;
	class ILogger;

	namespace video
	{
		class IVideoDriver;
	}
}

/*
    Remember to call
        driver->setMaterial(material);
        driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
    before calling dynamicsWorld->debugDrawWorld();
*/
class IPhysicsDebugDraw : public btIDebugDraw
{

public:

	IPhysicsDebugDraw(irr::IrrlichtDevice* const device);

    //////////////////////////
    // The 'dos' and 'sets' //
    //////////////////////////
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	void reportErrorWarning(const char* text);

	void draw3dText(const btVector3& location, const char* text);

	void setDebugMode(int mode) { this->mode = mode; }


    ////////////////
    // The 'gets' //
    ////////////////
	int getDebugMode() const { return this->mode; }

private:

	int mode;

	irr::video::IVideoDriver* const driver;

	irr::ILogger* logger;
};

#endif // __PHYSICS_DEBUG_H_INCLUDED__

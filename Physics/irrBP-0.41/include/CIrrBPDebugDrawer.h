#ifndef C_irrBP_DEBUG_DRAWER
#define C_irrBP_DEBUG_DRAWER

#include "LinearMath/btIDebugDraw.h"
#include "convert.h"
#include <irrlicht.h>

///Should not be used. Only for internal use.
class CIrrBPDebugDrawer : public btIDebugDraw
{
public:
	CIrrBPDebugDrawer(irr::video::IVideoDriver * driver);
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawTriangle(const btVector3& v0,const btVector3& v1,const btVector3& v2,const btVector3& color, btScalar alpha);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* text);
	void draw3dText(const btVector3& location, const char* text);
	void setDebugMode(int mode) { dmode = mode; }
	int getDebugMode() const { return dmode; }

private:
	int dmode;
	irr::video::IVideoDriver * vdriver;
	
};
#endif
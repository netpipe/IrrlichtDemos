#include "CIrrBPDebugDrawer.h"
#include <iostream>

using namespace bullet;

CIrrBPDebugDrawer::CIrrBPDebugDrawer(irr::video::IVideoDriver * driver)
{
	vdriver = driver;
	
}

void CIrrBPDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	irr::video::SColor irrcolor(255,color[0]*255,color[1]*255,color[2]*255);
	this->vdriver->draw3DLine(bullet::bulletVectorToIrrVector(from),bullet::bulletVectorToIrrVector(to),irrcolor);
}
void CIrrBPDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	irr::video::SColor irrcolor(255,color[0]*255,color[1]*255,color[2]*255);
	this->vdriver->draw3DLine(bullet::bulletVectorToIrrVector(PointOnB),bullet::bulletVectorToIrrVector(PointOnB + normalOnB * distance),irrcolor);
}
void CIrrBPDebugDrawer::reportErrorWarning(const char* text)
{
	std::cerr<<text<<std::endl;
}

void CIrrBPDebugDrawer::drawTriangle(const btVector3& v0,const btVector3& v1,const btVector3& v2,const btVector3& color, btScalar alpha)
{
	irr::video::SColor irrcolor(255,color[0]*255,color[1]*255,color[2]*255);
	this->vdriver->draw3DTriangle(irr::core::triangle3df(bullet::bulletVectorToIrrVector(v0),bullet::bulletVectorToIrrVector(v1),bullet::bulletVectorToIrrVector(v2)),irrcolor);
}
void CIrrBPDebugDrawer::draw3dText(const btVector3& location, const char* text)
{
	//Seems to not be called 
}
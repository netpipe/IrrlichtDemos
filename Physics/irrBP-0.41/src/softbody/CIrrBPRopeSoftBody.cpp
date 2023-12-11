#include <softbody/CIrrBPRopeSoftBody.h>
#include "CIrrBPWorld.h"

CIrrBPRopeSoftBody::~CIrrBPRopeSoftBody()
{
	delete m_softBody;
}
CIrrBPRopeSoftBody::CIrrBPRopeSoftBody(const irr::core::vector3df & From ,const irr::core::vector3df & To,irr::f32 mass,CIrrBPWorld * world,int res)
{
	from=From;
	to=To;
	driver = world->getIrrDevice()->getVideoDriver();
	draw = false;
	drawMat.Thickness = 3;
	drawMat.Lighting = false;
	m_softBody = btSoftBodyHelpers::CreateRope(world->getSoftBodyWorldInfo(),bullet::irrVectorToBulletVector(from),bullet::irrVectorToBulletVector(to),res == -1 ? getAutoFixedRes() : res,1);
	m_softBody->setTotalMass(mass);
	m_softBody->randomizeConstraints();
	collisionObj = m_softBody;
}

int CIrrBPRopeSoftBody::getAutoFixedRes()
{
	int distance = to.getDistanceFrom(from);
	return (distance/2);
}

void CIrrBPRopeSoftBody::update()
{
	if(!draw)
		return;
	driver->setTransform(irr::video::ETS_WORLD,irr::core::matrix4());
	driver->setMaterial(drawMat);
	for(int i=0;i<m_softBody->m_links.size();i++)
		driver->draw3DLine(bullet::bulletVectorToIrrVector(m_softBody->m_links[i].m_n[0]->m_x),bullet::bulletVectorToIrrVector(m_softBody->m_links[i].m_n[1]->m_x),irr::video::SColor(0,0,0,0));
}

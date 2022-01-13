#include "softbody/CIrrBPSoftBody.h"
#include <body/CIrrBPRigidBody.h>

using namespace bullet;

void CIrrBPSoftBody::addForce(const irr::core::vector3df& force)
{
	m_softBody->addForce(bullet::irrVectorToBulletVector(force));
}
void CIrrBPSoftBody::addForce(const irr::core::vector3df& force,int node)
{
	m_softBody->addForce(bullet::irrVectorToBulletVector(force),node);
}
void CIrrBPSoftBody::addVelocity(const irr::core::vector3df& velocity)
{
	m_softBody->addVelocity(bullet::irrVectorToBulletVector(velocity));
}
void CIrrBPSoftBody::setVelocity(const irr::core::vector3df& velocity)
{
	m_softBody->setVelocity(bullet::irrVectorToBulletVector(velocity));
}
void CIrrBPSoftBody::addVelocity(const irr::core::vector3df& velocity,int node)
{
	m_softBody->addVelocity(bullet::irrVectorToBulletVector(velocity),node);
}

void CIrrBPSoftBody::appendAnchor(CIrrBPRigidBody * body)
{
	int end = m_softBody->m_nodes.size()-1;
	appendAnchor(body,end);
}
void CIrrBPSoftBody::appendAnchor(CIrrBPRigidBody * body,int pos)
{
	m_softBody->appendAnchor(pos,body->getBodyPtr());
}
void CIrrBPSoftBody::setMass(irr::s32 node, irr::f32 mass)
{
	if(node > m_softBody->m_nodes.size())
		return;
	m_softBody->setMass(node,mass);
}

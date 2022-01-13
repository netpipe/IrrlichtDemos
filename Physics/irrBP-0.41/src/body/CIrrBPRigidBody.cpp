#include "body/CIrrBPRigidBody.h"
#include "animator/CIrrBPAnimator.h"

CIrrBPRigidBody::CIrrBPRigidBody()
{
	m_objType = RIGID_BODY;
	kinematic = false;
	m_Shape = NULL;
	m_MotionState = NULL;
	m_RigidBody = NULL;
}
CIrrBPRigidBody::~CIrrBPRigidBody()
{
	if(m_Shape)
	delete m_Shape;
	
	m_Shape = NULL;

	if(m_MotionState)
	delete m_MotionState;

	m_MotionState = NULL;

	if(m_RigidBody)
	delete m_RigidBody;
	
	m_RigidBody = NULL;
}
void CIrrBPRigidBody::activate(bool force)
{
	m_RigidBody->activate(force);
}
void CIrrBPRigidBody::applyCentralImpulse(const irr::core::vector3df &impulse)
{
	m_RigidBody->applyCentralImpulse(bullet::irrVectorToBulletVector(impulse));
}
void CIrrBPRigidBody::applyCentralForce(const irr::core::vector3df &force)
{
	m_RigidBody->applyCentralForce(bullet::irrVectorToBulletVector(force));
}
void  CIrrBPRigidBody::applyForce (const irr::core::vector3df &force, const irr::core::vector3df &rel_pos)
{
	m_RigidBody->applyForce(bullet::irrVectorToBulletVector(force),bullet::irrVectorToBulletVector(rel_pos));
}
void  CIrrBPRigidBody::applyTorqueImpulse (const irr::core::vector3df &torque)
{
	m_RigidBody->applyTorqueImpulse(bullet::irrVectorToBulletVector(torque));
}
void  CIrrBPRigidBody::applyImpulse (const irr::core::vector3df &impulse, const irr::core::vector3df &rel_pos) 
{
	m_RigidBody->applyImpulse(bullet::irrVectorToBulletVector(impulse),bullet::irrVectorToBulletVector(rel_pos));
}
void CIrrBPRigidBody::applyTorque (const irr::core::vector3df &torque)
{
	m_RigidBody->applyTorque(bullet::irrVectorToBulletVector(torque));
}

irr::f32 CIrrBPRigidBody::getAutomaticCCDSSR()
{
	irr::core::vector3df realExt = m_IrrSceneNode->getBoundingBox().getExtent() * m_IrrSceneNode->getScale();
	irr::f32 ccdSSR = 0.0f;
	irr::f32 tolerance = 2.0f;
	if(realExt.X < realExt.Y && realExt.X < realExt.Z)
		ccdSSR = realExt.X / tolerance;
	else if(realExt.Y < realExt.X && realExt.Y < realExt.Z)
		ccdSSR = realExt.Y / tolerance;
	else
		ccdSSR = realExt.Z / tolerance;
	return ccdSSR;
}

irr::f32 CIrrBPRigidBody::getAutomaticCCDMT()
{
	irr::core::vector3df realExt = m_IrrSceneNode->getBoundingBox().getExtent() * m_IrrSceneNode->getScale();
	irr::f32 ccdMT = 0.0f;
	irr::f32 tolerance = 1.2f;
	if(realExt.X < realExt.Y && realExt.X < realExt.Z)
		ccdMT = realExt.X / tolerance;
	else if(realExt.Y < realExt.X && realExt.Y < realExt.Z)
		ccdMT = realExt.Y / tolerance;
	else
		ccdMT = realExt.Z / tolerance;
	return ccdMT;
}


void CIrrBPRigidBody::setAutomaticCCD()
{
	this->setCcdMotionThreshold(this->getAutomaticCCDMT());
	this->setCcdSweptSphereRadius(this->getAutomaticCCDSSR());
}

irr::core::vector3df CIrrBPRigidBody::getPosition()
{
	return bullet::bulletVectorToIrrVector(m_RigidBody->getWorldTransform().getOrigin());
}

void CIrrBPRigidBody::setPosition(const irr::core::vector3df & newPos)
{
	m_IrrSceneNode->setPosition(newPos);
	btTransform  tr = m_RigidBody->getWorldTransform();
	tr.setOrigin(bullet::irrVectorToBulletVector(newPos));	
	m_RigidBody->setWorldTransform(tr);
}

irr::core::vector3df CIrrBPRigidBody::getRotation()
{
	return bullet::bulletTransformToIrrRotation(m_RigidBody->getWorldTransform());
}

void CIrrBPRigidBody::setRotation(const irr::core::vector3df & newPos)
{
	m_IrrSceneNode->setRotation(newPos);
	btTransform bt;
	bt = bullet::irrRotationToBulletTransform(newPos);
	bt.setOrigin(m_RigidBody->getWorldTransform().getOrigin());
	
	m_RigidBody->setWorldTransform(bt);
}

void CIrrBPRigidBody::setKinematic(bool isKinematic)
{
	kinematic = isKinematic;
	if(kinematic)
		this->m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	else
		this->m_RigidBody->setCollisionFlags(m_RigidBody->getCollisionFlags() ^ btCollisionObject::CF_KINEMATIC_OBJECT);

}
bool CIrrBPRigidBody::isKinematic()
{
	return kinematic;
}
void CIrrBPRigidBody::setDeactivation(bool value)
{
	deactivate = value;
	
	if(value)
		m_RigidBody->setActivationState(WANTS_DEACTIVATION);
	else
		m_RigidBody->setActivationState(DISABLE_DEACTIVATION);
}
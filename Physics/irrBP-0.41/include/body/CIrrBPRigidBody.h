#ifndef _CRIGIDBODY_H
#define _CRIGIDBODY_H

#include "CIrrBPCollisionObject.h"
#include <btBulletCollisionCommon.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>


class CIrrBPAnimator;

class CIrrBPRigidBody : public CIrrBPCollisionObject
{
public:
	CIrrBPRigidBody();
	virtual ~CIrrBPRigidBody();
	virtual void drop() = 0;

	virtual void  applyTorque (const irr::core::vector3df &torque) ;
	virtual void  applyForce (const irr::core::vector3df &force, const irr::core::vector3df &rel_pos) ;
	virtual void  applyCentralImpulse (const irr::core::vector3df &impulse) ;
	virtual void  applyCentralForce(const irr::core::vector3df &force);
	virtual void  applyTorqueImpulse (const irr::core::vector3df &torque) ;
	virtual void  applyImpulse (const irr::core::vector3df &impulse, const irr::core::vector3df &rel_pos) ;

	void activate(bool force=false);

	virtual btRigidBody * getBodyPtr() {return m_RigidBody;};
	virtual CMotionState * getMotionState() {return m_MotionState;}

	virtual bool isStaticObject() { return m_RigidBody->isStaticObject();}
	virtual irr::scene::ISceneNode * getIrrlichtNode() { return m_IrrSceneNode;}

	irr::core::vector3df getPosition();
	void setPosition(const irr::core::vector3df & newPos);

	irr::core::vector3df getRotation();
	void setRotation(const irr::core::vector3df & newPos);

	void setKinematic(bool isKinematic);
	bool isKinematic();

	//! If set to true, the body can be deactivated, otherwise not.
	void setDeactivation(bool value);

	bool canDeactivate(){return deactivate;}

	RIGID_BODY_TYPE getRigidBodyType(){return m_RBtype;}
protected:
	irr::f32 getAutomaticCCDSSR();
	irr::f32 getAutomaticCCDMT();
	void setAutomaticCCD();
	irr::scene::ISceneNode * m_IrrSceneNode;
	CMotionState * m_MotionState;
	btCollisionShape * m_Shape;
	btRigidBody * m_RigidBody;
	RIGID_BODY_TYPE m_RBtype;

	bool kinematic;
	bool deactivate;

};



#endif

#ifndef _CSOFTBODY_H
#define _CSOFTBODY_H

#include "CIrrBPCollisionObject.h"
#include <btBulletCollisionCommon.h>

#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>


class CIrrBPRigidBody;

class CIrrBPSoftBody : public CIrrBPCollisionObject
{
public:
	CIrrBPSoftBody(){m_objType = SOFT_BODY;}
	virtual void drop() = 0;
	virtual void addForce(const irr::core::vector3df& force);
	virtual void addForce(const irr::core::vector3df& force,int node);
	virtual void addVelocity(const irr::core::vector3df& velocity);
	virtual void setVelocity(const irr::core::vector3df& velocity);
	virtual void addVelocity(const irr::core::vector3df& velocity,int node);
	/*! Appends an anchor in the last cluster of the body*/
	void appendAnchor(CIrrBPRigidBody * body);
	/*! Appends an anchor to a specific cluster of the body*/
	void appendAnchor(CIrrBPRigidBody * body,int pos);
	/*! Sets a specific mass to a body's cluster.*/
	void setMass(irr::s32 node, irr::f32 mass);

	virtual btSoftBody * getBodyPtr() { return m_softBody;}

	/*! Gets the config structure. Useful to change soft body' nature.*/
	btSoftBody::Config getConfig() { return m_softBody->m_cfg;}

	/*! Sets a new config.*/
	void setConfig(btSoftBody::Config newConfig) { m_softBody->m_cfg = newConfig;}

	/*! Updates soft body triangles*/
	REALINLINE virtual void update() = 0;
protected:
	btSoftBody * m_softBody;
};

#endif

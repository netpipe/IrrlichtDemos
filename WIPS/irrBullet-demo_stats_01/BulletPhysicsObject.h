#pragma once

#include "btBulletDynamicsCommon.h"
#include "irrlicht.h"
using namespace irr;
using namespace core;


class CBulletPhysicsWorld;
class CCollisionObject;

class MyMotionState : public btDefaultMotionState
{
public:
	MyMotionState(const btTransform& startTrans = btTransform::getIdentity(),const btTransform& centerOfMassOffset = btTransform::getIdentity())
		: btDefaultMotionState(startTrans, centerOfMassOffset) {};

	//does this work?
	bool deactivationCallback(void*	userPointer) {
//		OutputDebugString("deactivating ");
		return true;
	}
};


class CBulletPhysicsObject {
private:

	//there is a deactivation callback in the btMotionState class
	MyMotionState* m_pMotionState;

protected:
	CCollisionObject* m_pCollisionObject;
	f32 m_Mass;

public:
	btRigidBody* m_pRigidBody;
	static CBulletPhysicsWorld* s_pPhysicsWorld;

protected:
	void QuaternionToEulerXYZ(const btQuaternion &quat,btVector3 &euler);
	void EulerXYZToQuaternion(btVector3 &euler, btQuaternion &quat);
	btTriangleMesh* GetTriangleMesh(scene::IMesh* pIMesh);


public:
	CBulletPhysicsObject(void);
	~CBulletPhysicsObject(void);
	void ApplyForce(const vector3df & v);
	void InitPhysics(f32 ccdThreshold,f32 linearDamping=0.f, f32 angularDamping=0.f, f32 friction=0.5f, f32 restitution=0.f);
	virtual void SetPosition(const vector3df & v);
	virtual void SetRotation(const vector3df & v);
	vector3df GetLinearVelocity() const;
	void SetLinearVelocity(const vector3df & vel) const;
	vector3df GetAngularVelocity() const;
	void SetAngularVelocity(const vector3df & vel) const;
	void ZeroForces();
	void SetActivationState(bool active);
	vector3df GetRotation();
	vector3df GetPosition();
	void EndPhysics(void);
};

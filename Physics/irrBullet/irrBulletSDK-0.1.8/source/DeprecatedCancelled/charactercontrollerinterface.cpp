// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see copyright notice in irrbullet.h

#include "charactercontrollerinterface.h"
#include "irrBullet.h"
#include "Bullet/BulletCollision/CollisionDispatch/btGhostObject.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
{
public:
    ClosestNotMe (btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
    {
        m_me = me;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == m_me)
            return 1.0;

        return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
    }

protected:
    btRigidBody* m_me;
};

ICharacterControllerInterface::ICharacterControllerInterface(IRigidBody* const body, btCollisionShape* collShape,
    btPairCachingGhostObject* ghostObject, irr::f32 stepHeight) : btKinematicCharacterController(ghost, collShape, stepHeight)
{
    static u32 characterControllerID;
    characterControllerID++;

    rigidBody = body;

    uniqueID = characterControllerID;

	btTransform transform;
	btTransformFromIrrlichtMatrix(body->getWorldTransform(), transform);

	ghostObject->setWorldTransform(transform);

	/*m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(transform);
	m_shape = new btCapsuleShape(1.75,1.75);
	m_ghostObject->setCollisionShape(capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	//btScalar stepHeight = btScalar(0.35);

	btPairCachingGhostObject* ghost = new btPairCachingGhostObject();*/
	this = new btKinematicCharacterController(m_ghostObject,
        m_shape, stepHeight);

    configure();
}

void ICharacterControllerInterface::configure()
{
    //rigidBody->setSleepingThresholds(0.0f, 0.0f);
	//rigidBody->setAngularFactor(0.0f);
}

void ICharacterControllerInterface::preStep(const btCollisionWorld* collisionWorld)
{
    btTransform xform;
	rigidBody->getPointer()->getMotionState()->getWorldTransform(xform);
	btVector3 down = -xform.getBasis()[1];
	btVector3 forward = xform.getBasis()[2];
	down.normalize ();
	forward.normalize();

	raySource[0] = xform.getOrigin();
	raySource[1] = xform.getOrigin();

	rayTarget[0] = raySource[0] + down * halfHeight * btScalar(1.1);
	rayTarget[1] = raySource[1] + forward * halfHeight * btScalar(1.1);

	ClosestNotMe rayCallback(rigidBody->getPointer());

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		rayCallback.m_closestHitFraction = 1.0;
		collisionWorld->rayTest (raySource[i], rayTarget[i], rayCallback);
		if (rayCallback.hasHit())
		{
			rayLambda[i] = rayCallback.m_closestHitFraction;
		}
		else
		{
			rayLambda[i] = 1.0;
		}
	}
}

void ICharacterControllerInterface::playerStep(const btCollisionWorld* dynaWorld, btScalar dt,
    int forward,
    int backward,
    int left,
    int right,
    int jump)
{
    btTransform xform;
	rigidBody->getPointer()->getMotionState()->getWorldTransform(xform);

	if(left)
		turnAngle -= dt * turnVelocity;
	if(right)
		turnAngle += dt * turnVelocity;

	xform.setRotation (btQuaternion(btVector3(0.0, 1.0, 0.0), turnAngle));

	btVector3 linearVelocity = rigidBody->getPointer()->getLinearVelocity();
	btScalar speed = rigidBody->getPointer()->getLinearVelocity().length();

	btVector3 forwardDir = xform.getBasis()[2];
	forwardDir.normalize ();
	btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
	btScalar walkSpeed = walkVelocity * dt;

	if (forward)
		walkDirection += forwardDir;
	if (backward)
		walkDirection -= forwardDir;



	if (!forward && !backward && onGround())
	{
		linearVelocity *= btScalar(0.2);
		rigidBody->getPointer()->setLinearVelocity(linearVelocity);
	}
	else
	{
		if (speed < maxLinearVelocity)
		{
			btVector3 velocity = linearVelocity + walkDirection * walkSpeed;
			rigidBody->getPointer()->setLinearVelocity(velocity);
		}
	}

	rigidBody->getPointer()->getMotionState()->setWorldTransform(xform);
	rigidBody->getPointer()->setCenterOfMassTransform(xform);
}

void ICharacterControllerInterface::jump()
{
	if (!canJump())
		return;

	btTransform xform;
	rigidBody->getPointer()->getMotionState()->getWorldTransform(xform);
	btVector3 up = xform.getBasis()[1];
	up.normalize ();
	btScalar magnitude = (btScalar(1.0)/rigidBody->getPointer()->getInvMass()) * btScalar(8.0);
	rigidBody->getPointer()->applyCentralImpulse(up * magnitude);
}

void ICharacterControllerInterface::reset()
{
}
void ICharacterControllerInterface::warp(const btVector3& origin)
{
}
void ICharacterControllerInterface::registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher)
{
}

ICharacterControllerInterface::~ICharacterControllerInterface()
{

}

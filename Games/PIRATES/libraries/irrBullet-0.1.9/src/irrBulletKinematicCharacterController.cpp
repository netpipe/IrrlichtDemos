// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#include "irrBulletKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "irrBullet.h"

IKinematicCharacterController::IKinematicCharacterController(irrBulletWorld* const world, irr::f32 height, irr::f32 width, irr::f32 stepHeight) : World(world)
{
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.210098, 100.6433364, 1.453260));

	GhostObject = new btPairCachingGhostObject();
	GhostObject->setWorldTransform(startTransform);
	btScalar characterHeight = height;
	btScalar characterWidth = width;
	Capsule = new btCapsuleShape(characterWidth, characterHeight);
	GhostObject->setCollisionShape(Capsule);
	GhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	Character = new btKinematicCharacterController(GhostObject, Capsule, btScalar(stepHeight));
	
	World->getPointer()->addCollisionObject(GhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	World->getPointer()->addAction(Character);
}

void IKinematicCharacterController::setWalkDirection(const irr::core::vector3df& dir)
{
	Character->setWalkDirection(irrlichtToBulletVector(dir));
}

void IKinematicCharacterController::reset()
{
	Character->reset(static_cast<btCollisionWorld*>(World->getPointer()));
}

void IKinematicCharacterController::warp(const irr::core::vector3df& origin)
{
	Character->warp(irrlichtToBulletVector(origin));
}


void IKinematicCharacterController::preStep(irrBulletWorld* world)
{
	Character->preStep(static_cast<btCollisionWorld*>(world->getPointer()));
}

void IKinematicCharacterController::playerStep(irrBulletWorld* world, irr::f32 dt)
{
    Character->playerStep(static_cast<btCollisionWorld*>(world->getPointer()), btScalar(dt));
}

void IKinematicCharacterController::setFallSpeed(irr::f32 fallSpeed)
{
	Character->setFallSpeed(fallSpeed);
}

void IKinematicCharacterController::setJumpForce(irr::f32 jumpForce)
{
	Character->setJumpSpeed(jumpForce);
}

void IKinematicCharacterController::setMaxJumpHeight(irr::f32 maxJumpHeight)
{
	Character->setMaxJumpHeight(maxJumpHeight);
}

void IKinematicCharacterController::jump()
{
	Character->jump();
}

void IKinematicCharacterController::setUseGhostSweepTest(bool useGhostObjectSweepTest)
{
	Character->setUseGhostSweepTest(useGhostObjectSweepTest);
}

void IKinematicCharacterController::setUpAxis(irr::u32 axis)
{
	Character->setUpAxis(axis);
}

void IKinematicCharacterController::setPositionIncrementPerSimulatorStep(const irr::core::vector3df& increment)
{
	Character->setWalkDirection(irrlichtToBulletVector(increment));
}

void IKinematicCharacterController::setVelocityForTimeInterval(const irr::core::vector3df& velocity, irr::f32 timeInterval)
{
	Character->setVelocityForTimeInterval(irrlichtToBulletVector(velocity), timeInterval);
}

void IKinematicCharacterController::setGravity(irr::f32 gravity)
{
	Character->setGravity(gravity);
}

void IKinematicCharacterController::setMaxSlope(irr::f32 slopeRadians)
{
	Character->setMaxSlope(slopeRadians);
}

irr::core::matrix4 IKinematicCharacterController::getWorldTransform()
{
	irr::core::matrix4 mat;
	btTransformToIrrlichtMatrix(GhostObject->getWorldTransform(), mat);

	return mat;
}

bool IKinematicCharacterController::canJump() const
{
	return Character->canJump();
}

btPairCachingGhostObject* IKinematicCharacterController::getGhostObject() const
{
	return Character->getGhostObject();
}

bool IKinematicCharacterController::isOnGround() const
{
	return Character->onGround();
}

void IKinematicCharacterController::setUpInterpolate(bool value)
{
	Character->setUpInterpolate(value);
}
irr::f32 IKinematicCharacterController::getGravity() const
{
	return Character->getGravity();
}

irr::f32 IKinematicCharacterController::getMaxSlope() const
{
	return Character->getMaxSlope();
}

IKinematicCharacterController::~IKinematicCharacterController()
{
	delete Capsule;
	World->getPointer()->removeCollisionObject(GhostObject);
	World->getPointer()->removeAction(Character);
}

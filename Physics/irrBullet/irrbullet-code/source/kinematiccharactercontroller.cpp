#include "kinematiccharactercontroller.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "irrBullet.h"

IKinematicCharacterController::IKinematicCharacterController(irrBulletWorld* const world) : World(world)
{
    btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.210098,100.6433364,1.453260));

	GhostObject = new btPairCachingGhostObject();
	GhostObject->setWorldTransform(startTransform);
	btScalar characterHeight = 3.0;
	btScalar characterWidth = 1.95;
	Capsule = new btCapsuleShape(characterWidth,characterHeight);
	GhostObject->setCollisionShape(Capsule);
	GhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = btScalar(0.35);
	Character = new btKinematicCharacterController(GhostObject,Capsule,stepHeight);

	World->getPointer()->addCollisionObject(GhostObject,btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);

	World->getPointer()->addAction(Character);
}

void IKinematicCharacterController::reset()
{
    Character->reset();
}

void IKinematicCharacterController::warp(const irr::core::vector3df& origin)
{
    Character->warp(irrlichtToBulletVector(origin));
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



const irr::core::matrix4 IKinematicCharacterController::getWorldTransform() const
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

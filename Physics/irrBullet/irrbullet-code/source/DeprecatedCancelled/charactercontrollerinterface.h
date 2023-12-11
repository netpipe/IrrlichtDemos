#ifndef __CHARACTER_CONTROLLER_INTERFACE_H_INCLUDED__
#define __CHARACTER_CONTROLLER_INTERFACE_H_INCLUDED__

#include "Bullet/BulletCollision/CollisionShapes/btConvexShape.h"
#include "Bullet/BulletDynamics/Character/btKinematicCharacterController.h"
#include "Bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "Bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"

#include <irrTypes.h>
#include <vector3d.h>
#include "collisionshape.h"
#include "rigidbody.h"


class ICharacterControllerInterface : public btCharacterControllerInterface
{
    public:
        ICharacterControllerInterface(IRigidBody* const body);
        virtual ~ICharacterControllerInterface();

        void configure();

        //virtual void setWalkDirection(const btVector3& walkDirection) {};

        //virtual void setVelocityForTimeInterval(const btVector3 &velocity, btScalar timeInterval) {};

        // Operators
        bool operator==(const ICharacterControllerInterface& other) const { return (this->uniqueID == other.getUniqueID()); };
        bool operator!=(const ICharacterControllerInterface& other) const { return !(*this == other); }

        //btKinematicCharacterController* getPointer() { return character; };

        btPairCachingGhostObject* getGhostObject() { return m_ghostObject; };

        irr::u32 getUniqueID() const { return uniqueID; };

        virtual void registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher);

        btCollisionObject* getCollisionObject ();

        virtual void setWalkDirection(const btVector3& dir) { walkDirection = dir; };
        virtual void setVelocityForTimeInterval(const btVector3& velocity, btScalar timeInterval);
        virtual void reset();
        virtual void warp (const btVector3& origin);

        virtual void preStep(btCollisionWorld* collisionWorld);
        virtual void playerStep (btCollisionWorld* collisionWorld, btScalar dt);
        virtual bool canJump() const;
        virtual void jump();

        virtual bool onGround() const;

    protected:
        btPairCachingGhostObject* m_ghostObject;
        btCollisionShape* m_shape;
        IRigidBody* rigidBody;

        btScalar halfHeight;

        btVector3 walkDirection;

        btVector3 raySource[2];
        btVector3 rayTarget[2];
        btScalar rayLambda[2];
        btVector3 rayNormal[2];

        btScalar turnAngle;

        btScalar maxLinearVelocity;
        btScalar walkVelocity;
        btScalar turnVelocity;

        irr::u32 uniqueID;
};

#endif // __CHARACTER_CONTROLLER_INTERFACE_H_INCLUDED__

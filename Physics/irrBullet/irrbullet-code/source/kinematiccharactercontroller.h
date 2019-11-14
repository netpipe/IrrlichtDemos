#ifndef __KINEMATIC_CHARACTER_CONTROLLER_H__
#define __KINEMATIC_CHARACTER_CONTROLLER_H__

#include <irrTypes.h>
#include <vector3d.h>
#include <matrix4.h>

class irrBulletWorld;

class btConvexShape;
class btPairCachingGhostObject;
class btKinematicCharacterController;

/*!
    The IKinematicCharacterController is an object that supports a sliding motion in a world.
    It uses a ghost object and convex sweep test to test for upcoming collisions. This is combined with discrete collision detection to recover from penetrations.
*/
class IKinematicCharacterController
{
    public:
        IKinematicCharacterController(irrBulletWorld* const world);
        ~IKinematicCharacterController();

        void reset();

        void warp(const irr::core::vector3df& origin);

        void setFallSpeed(irr::f32 fallSpeed);

        void setJumpForce(irr::f32 jumpForce);

        /// If the character reaches this height from where it starts jumping, it will begin to fall.
        void setMaxJumpHeight(irr::f32 maxJumpHeight);

        void jump();

        void setUseGhostSweepTest(bool useGhostObjectSweepTest);

        void setUpAxis(irr::u32 axis);

        /*!
            This is neither a direction nor a velocity, but the amount to
            increment the position each simulation iteration, regardless of deltatime.
            This call will reset any velocity set by setVelocityForTimeInterval().
        */
        void setPositionIncrementPerSimulatorStep(const irr::core::vector3df& increment);

        /*!
            The caller provides a velocity with which the character should move for
            the given time period.  After the time period, velocity is reset to zero.
            This call will reset any walk direction set by setPositionIncrementPerSimulatorStep().
            Negative time intervals will result in no motion.
        */
        void setVelocityForTimeInterval(const irr::core::vector3df& velocity, irr::f32 timeInterval);

        void setGravity(irr::f32 gravity);

        /*!
            The max slope determines the maximum angle that the controller can walk up.
            The slope angle is measured in radians.
        */
        void setMaxSlope(irr::f32 slopeRadians);




        btPairCachingGhostObject* getGhostObject() const;

        /// Return the world transform of the character
        const irr::core::matrix4 getWorldTransform() const;

        bool isOnGround() const;

        bool canJump() const;

        irr::f32 getGravity() const;

        irr::f32 getMaxSlope() const;

    private:
        irrBulletWorld* World;
        btConvexShape* Capsule;
        btPairCachingGhostObject* GhostObject;
        btKinematicCharacterController* Character;
        irr::core::matrix4 WorldTransform;
};

#endif // __KINEMATIC_CHARACTER_CONTROLLER_H__

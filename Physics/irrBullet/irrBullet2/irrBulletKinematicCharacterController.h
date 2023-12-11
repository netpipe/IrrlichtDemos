// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include <irrTypes.h>
#include <vector3d.h>
#include <matrix4.h>
#include <irrlicht.h>

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
	/*!
		Default constructor that uses btCapsuleShape for approximating character - fast
	*/
	IKinematicCharacterController(irrBulletWorld* const world, irr::f32 height = 5.0f, irr::f32 width = 2.0f, irr::f32 stepHeight = 1.0f);

	~IKinematicCharacterController();

	void setWalkDirection(const irr::core::vector3df& dir);

	void reset();

	void warp(const irr::core::vector3df& origin);

	void preStep(irrBulletWorld* world);

	void playerStep(irrBulletWorld* world, irr::f32 dt);

	void setFallSpeed(irr::f32 fallSpeed);

	void setJumpForce(irr::f32 jumpForce);

	/// If the character reaches this height from where it starts jumping, it will begin to fall.
	void setMaxJumpHeight(irr::f32 maxJumpHeight);

	void jump();

	void setUseGhostSweepTest(bool useGhostObjectSweepTest);

	void updateAction(irrBulletWorld* world, irr::f32 dt)
	{
		preStep(world);
		playerStep(world, dt);
	}

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

	/*!
	set Bullet world gravity, default to the one provided with irrBulletWorld
	@param gravity Y-axis of gravitational force
	*/

	void setGravity(irr::f32 gravity);

	/*!
		The max slope determines the maximum angle that the controller can walk up.
		The slope angle is measured in radians.
		*/
	void setMaxSlope(irr::f32 slopeRadians);

	btPairCachingGhostObject* getGhostObject() const;

	/// Return the world transform of the character
	irr::core::matrix4 getWorldTransform();

	bool isOnGround() const;

	void setUpInterpolate(bool value);

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

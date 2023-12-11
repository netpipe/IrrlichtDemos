
#include "Character.h"
#include "IMesh.h"
#include "IWorld.h"
#include "IPhysics.h"
#include "IInputProfile.h"
#include "IDynamicBody.h"
#include "ISphereCollisionGeometry.h"
#include "ICharacterController.h"

// Fixes needed:
// - PID controller to maintain orientation when on a slope? (created when keys not pressed)
// - user mass setting?
// - material? default material? user material? what about interactions?
//		- material might NOT want setting.... as not all character models rely on this (e.g. lollipop)
//		- and this is really an internal setting...
// - user damping amount setting?

Character::Character(IWorld *world, IPhysics *physics, IMesh *mesh, s32 idAnimIdle, s32 idAnimWalk, f32 height)
{
	this->world = world;
	this->mesh = mesh;
	
	// Animations
	SetAnimations(idAnimIdle, idAnimWalk);
	
	SetMoveVec(core::vector2df(0,1));
	ClearMotion();
	
	// default move speed
	SetMoveSpeed(10.0);
	
	// Default turn smoothing
	SetTurnSmoothing(10.0);
	
	body = physics->AddDynamicBody();
	
	if (height == 0.0)
	{
		// No height given, so get the geometry from the bounding box.
		geom = physics->CreateSphereCollisionGeometryFromBB(mesh);
	}
	else
	{
		// Get a sphere of radius half the height.
		geom = physics->CreateSphereCollisionGeometry(height / 2.0);
	}
	
	body->AddCollisionGeometry(geom);
	
	// default mass
	SetDensity(0.01);
	
	// High damping so it comes to a stop quickly
	body->SetAngularDamping(0.95);
	
	// Default animation.
	mesh->SetAnimation(idAnimIdle);
	
	// Offset the mesh so that its bottom is level with the bottom of the geometry.
	f32 geomHeight = geom->GetSize().Y;
	f32 meshSink = -mesh->GetBoundingBox().MinEdge.Y; // amount by which the mesh encroaches under zero Y.
	meshHeightOffset = meshSink-geomHeight/2.0;
}

Character::~Character()
{
	geom->drop();
	
	world->RemoveTransformable(mesh);
	world->RemoveTransformable(body);
}

void Character::SetPosition(const core::vector3df &pos)
{
	body->SetPosition(pos);
	mesh->SetPosition(pos + core::vector3df(0,meshHeightOffset,0));
}

void Character::SetRotation(const core::vector3df &rot)
{
	mesh->SetRotation(rot);
}

core::vector3df Character::GetPosition()
{
	return body->GetPosition();
}

core::vector3df Character::GetRotation()
{
	return mesh->GetRotation();
}

void Character::SetAnimations(s32 idAnimIdle, s32 idAnimWalk)
{
	this->idAnimIdle = idAnimIdle;
	this->idAnimWalk = idAnimWalk;
}

bool Character::IsSetAnimations(s32 idAnimIdle, s32 idAnimWalk)
{
	return this->idAnimIdle == idAnimIdle
			&& this->idAnimWalk == idAnimWalk;
}

void Character::SetGravityEnabled(bool enabled)
{
	body->SetGravityEnabled(enabled);
}

void Character::SetDensity(f32 density)
{
	body->SetMass(density, geom);
}

void Character::SetMaterial(ICollisionMaterial *material)
{
	geom->SetMaterial(material);
}

void Character::SetMoveSpeed(f32 speed)
{
	moveSpeed = speed;
}

void Character::SetMoveVec(const core::vector2df &vec)
{
	moveVec = vec;
	moveVec.normalize();
}

core::vector2df Character::GetMoveVec()
{
	return moveVec;
}

void Character::SetMedialMotion(c8 dir)
{
	relativeMotion.Y = ClampDir(dir);
}

void Character::SetLateralMotion(c8 dir)
{
	relativeMotion.X = ClampDir(dir);
}

core::vector3df Character::GetAttemptedMove()
{
	return attemptedMove;
}

void Character::StopMoving()
{
	SetMedialMotion(0);
	SetLateralMotion(0);
	body->ZeroAngularVelocity();
	
	// Excluding Y, don't want to stop falling.
	body->ZeroLinearVelocity(AXIS_X|AXIS_Z);
}

void Character::Update(f32 dt)
{
	IInputProfile *inputProfile = world->GetInputProfile();
	
	if (!inputProfile)
		return;
	
	// Find angle of movement
	// Defaults to the last set (by SetMoveVec) angle of movement.
	f32 faceAngle = maths::rotation_required_2d(moveVec);
	
	// Actual vector to move in.
	// This defaults to zero motion.
	// (e.g. no keys were pressed to move the character)
	core::vector2df actualMotion;
	
	// Should motion actually occur?
	// This is motion due to character control (key input, AI)
	if (relativeMotion.getLengthSQ() != 0.0)
	{
		// So we face in the direction of character control
		// (e.g. motion left, face left)
		faceAngle += maths::rotation_required_2d(relativeMotion);
		
		// Some actual motion occurs
		actualMotion = core::vector2df( maths::vec_from_angle(faceAngle) );
	}
	
	// Set correct animation.
	// Walk if moving.
	if (relativeMotion.getLengthSQ() != 0.0)
			// this would enable walking while auto-rotating
			// however auto-rotating has been removed
			//|| rotation_difference(GetRotation(),faceAngle) > 0.5)
	{
		mesh->SetAnimation(idAnimWalk);
	}
	else
	{
		mesh->SetAnimation(idAnimIdle);
	}
	
	if (relativeMotion.getLengthSQ() != 0.0)
	{
		// Move the physics body.
		// (X and Z are switched because rotation about X axis causes movement along Z, and vice versa)
		body->ApplyTorque( core::vector3df(actualMotion.Y,0.0,actualMotion.X) * moveSpeed);
		
		// If character IS being moved, then change rotation to facing angle.
		// Keep only the Y component of character's current rotation
		
		if (turnSmooth == 0.0) // smooth interpolated movement?
			SetRotation( core::vector3df(0,faceAngle,0) );
		else
		{
			f32 currentRotY = maths::extract_y_rotation(GetRotation());
			
			SetRotation(core::vector3df(0,
					maths::interpolate_angle( currentRotY, faceAngle, turnSmooth*dt ),
					0));
		}
	}
	
	// Set attempted move.
	attemptedMove.X = actualMotion.X;
	attemptedMove.Y = 0.0;
	attemptedMove.Z = actualMotion.Y;
	
	// Update mesh position from body
	mesh->SetPosition(body->GetPosition() + core::vector3df(0,meshHeightOffset,0));
}



#ifndef I_CHARACTER_H
#define I_CHARACTER_H

#include "ITransformable.h"
#include "ICharacterController.h"

class ICollisionMaterial;
class IDynamicBody;

class ICharacter : public ITransformable
{
	ICharacterController *controller;
	
public:
	ICharacter()
	{
		controller = NULL;
	}
	
	virtual ~ICharacter()
	{
		if (controller)
			controller->drop();
	}
	
	void SetController(ICharacterController *controller)
	{
		SET_REF_COUNTED_POINTER(this->controller, controller)
	}
	
	ICharacterController *GetController()
	{
		return controller;
	}
	
	// Set which animations of the mesh to use for the given actions.
	virtual void SetAnimations(s32 idAnimIdle, s32 idAnimWalk) = 0;
	
	// Returns true if these animations have been set.
	virtual bool IsSetAnimations(s32 idAnimIdle, s32 idAnimWalk) = 0;
	
	// default true.
	// Simply wraps the IDynamicBody method.
	virtual void SetGravityEnabled(bool enabled) = 0;
	
	// Set the density of the character. Default 0.01.
	// Note that the actual weight of the character will also depend
	// on the size of the character mesh.
	// So characters with the same density but different sized meshes may
	// all have different masses, and thus may move at different speeds with 
	// the application of identical forces.
	virtual void SetDensity(f32 density) = 0;
	
	// set a material... (otherwise uses default physics material)
	virtual void SetMaterial(ICollisionMaterial *material) = 0;
	
	// Default 10.0
	// Defines force or torque used to move the character.
	virtual void SetMoveSpeed(f32 speed) = 0;
	
	// Direction in the horizontal plane that the character is facing.
	// Movement occurs relative to this.
	// This method may be used internally by character controllers.
	virtual void SetMoveVec(const core::vector2df &vec) = 0;
	
	// Get the move vector as set by SetMoveVec
	virtual core::vector2df GetMoveVec() = 0;
	
	virtual void ClearMotion()
	{
		SetMedialMotion(0);
		SetLateralMotion(0);
	}
	
	// only the sign is used. Positive for forwards movement, negative for backwards, zero for no movement
	virtual void SetMedialMotion(c8 dir) = 0;
	
	// negative for left, positive for right, zero for no movement
	virtual void SetLateralMotion(c8 dir) = 0;
	
	// A value used for smooth rotation of the character when turning to a new angle.
	// Default 10.0
	// Set to 0 to disable (immediately looks to new angle).
	virtual void SetTurnSmoothing(f32 smooth) = 0;
	
	// Get a normalised vector of the attempted motion of the character in the last update.
	// This is a result of control (AI or key input), the move vector, and medial or lateral motions.
	// This is the *attempted* move, not the actual move achieved (the move may be obstructed by a collision).
	// Does not consider "accidental" movement due to external things like physics.
	virtual core::vector3df GetAttemptedMove() = 0;
	
	// zero velocities and stop motion
	virtual void StopMoving() = 0;
	
	// get the character mesh instance
	virtual IMesh *GetMesh() = 0;
	
	// get character body...
	virtual IDynamicBody *GetBody() = 0;
	
	// Tick
	virtual void Update(f32 dt) = 0;
};

#endif

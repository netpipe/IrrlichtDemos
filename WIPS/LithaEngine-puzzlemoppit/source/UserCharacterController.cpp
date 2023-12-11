
#include "UserCharacterController.h"
#include "IWorld.h"
#include "ICharacter.h"
#include "IInputProfile.h"

UserCharacterController::UserCharacterController(IWorld *world,
		int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
		ITransformable *moveRelativeTo)
{
	this->world = world;
	
	this->idButtonWalkForwards	= idButtonWalkForwards;
	this->idButtonWalkBackwards	= idButtonWalkBackwards;
	this->idButtonWalkLeft		= idButtonWalkLeft;
	this->idButtonWalkRight		= idButtonWalkRight;
	
	this->moveRelativeTo = moveRelativeTo;
}

void UserCharacterController::Update(ICharacter *character)
{
	if (moveRelativeTo)
	{
		character->SetMoveVec( maths::get_horizontal_vec(moveRelativeTo->GetRotation()) );
	}
	
	IInputProfile *inputProfile = world->GetInputProfile();
	
	if (!inputProfile)
		return;
	
	// Start with no motion...
	
	character->ClearMotion();
	
	// Maybe set medial motion
	
	if (inputProfile->IsButtonDown(idButtonWalkForwards)
			&& !inputProfile->IsButtonDown(idButtonWalkBackwards))
	{
		character->SetMedialMotion(1);
	}
	
	if (inputProfile->IsButtonDown(idButtonWalkBackwards)
			&& !inputProfile->IsButtonDown(idButtonWalkForwards))
	{
		character->SetMedialMotion(-1);
	}
	
	// Maybe set lateral motion
	
	if (inputProfile->IsButtonDown(idButtonWalkLeft)
			&& !inputProfile->IsButtonDown(idButtonWalkRight))
	{
		character->SetLateralMotion(-1);
	}
	
	if (inputProfile->IsButtonDown(idButtonWalkRight)
			&& !inputProfile->IsButtonDown(idButtonWalkLeft))
	{
		character->SetLateralMotion(1);
	}
}


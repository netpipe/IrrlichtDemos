
#ifndef USER_CHARACTER_CONTROLLER_H
#define USER_CHARACTER_CONTROLLER_H

#include "ICharacterController.h"

class IWorld;
class ITransformable;

class UserCharacterController : public ICharacterController
{
	IWorld *world;
	
	int idButtonWalkForwards;
	int idButtonWalkBackwards;
	int idButtonWalkLeft;
	int idButtonWalkRight;
	
	ITransformable *moveRelativeTo;
	
public:
	UserCharacterController(IWorld *world,
			int idButtonWalkForwards,int idButtonWalkBackwards, int idButtonWalkLeft, int idButtonWalkRight,
			ITransformable *moveRelativeTo);
	
	void Update(ICharacter *character);
};

#endif

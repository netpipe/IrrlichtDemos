
#ifndef I_CHARACTER_CONTROLLER_H
#define I_CHARACTER_CONTROLLER_H

#include "litha_internal.h"

class ICharacter;

class ICharacterController : public virtual IReferenceCounted
{
public:
	virtual ~ICharacterController(){}
	
	virtual void Update(ICharacter *character) = 0;
};

#endif


#ifndef BUTTONS_H
#define BUTTONS_H

#include "litha_internal.h"

struct ButtonStates
{
	bool states[KEY_KEY_CODES_COUNT];
	
	void Clear()
	{
		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i ++)
			states[i] = false;
	}
	
	bool &operator [](u32 index)
	{
		if (index >= KEY_KEY_CODES_COUNT)
		{
			FAIL << "Invalid button " << index;
		}
		return states[index];
	}
	
	const bool &operator [](u32 index) const
	{
		if (index >= KEY_KEY_CODES_COUNT)
		{
			FAIL << "Invalid button " << index;
		}
		return states[index];
	}
};

#endif

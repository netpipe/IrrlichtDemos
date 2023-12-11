
#ifndef I_PAUSABLE_H
#define I_PAUSABLE_H

#include "litha_internal.h"

class IPausable
{
	u32 pauseCounter;
	
protected:

	// Actual implementation of what should occur when an object is paused.
	virtual void OnPause() = 0;
	
	// Actual implementation of what should occur when an object is resumed.
	virtual void OnResume() = 0;
	
public:
	IPausable()
	{
		pauseCounter = 0;
	}
	
	virtual ~IPausable(){}
	
	// If you call Pause, you must also call Resume at a later date, since it is reference counted.
	// e.g. if Pause is called twice, in two different places, the object will not be un-paused until
	// Resume has been called twice.
	void Pause()
	{
		if (pauseCounter == 0)
			OnPause();
		
		pauseCounter ++;
	}
	
	// This should only be called if Pause has previously been called.
	void Resume()
	{
		ASSERT(pauseCounter > 0);
		
		pauseCounter --;
		
		if (pauseCounter == 0)
			OnResume();
	}
	
	bool IsPaused() const
	{
		return pauseCounter > 0;
	}
};

#endif


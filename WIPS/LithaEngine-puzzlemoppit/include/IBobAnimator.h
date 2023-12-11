
#ifndef I_BOB_ANIMATOR
#define I_BOB_ANIMATOR

#include "IMotionAnimator.h"

class IBobAnimator : public IMotionAnimator
{
public:
	virtual ~IBobAnimator(){}
	
	// Should bob about a fixed position? (object's position when Init is called)
	// Otherwise, will just add the delta each time, but may eventually drift due to inaccuracies.
	// Default true.
	virtual void SetFixed(bool fixed) = 0;
	
	// Change the start position (defaults to the position of the target when Init is called).
	// Only relevant if SetFixed(true)
	virtual void SetStartPos(const core::vector3df &pos) = 0;
};

#endif


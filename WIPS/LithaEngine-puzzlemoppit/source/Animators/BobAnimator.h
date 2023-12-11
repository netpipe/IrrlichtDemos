
#include "IBobAnimator.h"

class IWorld;

class BobAnimator : public IBobAnimator
{
	IWorld *world;
	
	bool isFixed;
	
	core::vector3df startPos;
	core::vector3df lastPos;
	
	f32 startTime;
	f32 amplitude;
	f32 speed;
	
public:
	// Degrees / second
	BobAnimator(f32 amplitude, f32 speed);
	
	void Init(ITransformable *target);
	void Animate(ITransformable *target, f32 dt);
	
	// Should bob about a fixed position? (object's position when Init is called)
	// Otherwise, will just add the delta each time, but may eventually drift due to inaccuracies.
	void SetFixed(bool fixed) { isFixed = fixed; }
	
	// Change the start position (defaults to the position of the target when Init is called).
	// Only relevant if SetFixed(true)
	void SetStartPos(const core::vector3df &pos) { startPos = pos; }
};



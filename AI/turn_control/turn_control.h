#ifndef _TURN_CONTROL_H_
#define _TURN_CONTROL_H_

#include <vector>
#include <irrlicht.h>


using namespace irr;


#ifndef TURN_CONTROL_PROPERTIES
#define TURN_CONTROL_PROPERTIES
#define RATE_CONSTANT 1.0f
#define STOPPING_INTERPOLATE 0.99f
#endif




class TurnControl
{

	public:
	
	TurnControl
	(
		IrrlichtDevice* device,
		scene::ISceneNode* node
	);
	virtual ~TurnControl();

	static
	bool getRelativeHeadingDegs
	(
		scene::ISceneNode* node,
		scene::ISceneNode* target,
		f32& degs
	);
	
	static
	core::vector3df getHeading(scene::ISceneNode* node);
	
	void turnTo(scene::ISceneNode* target);
	
	void update();
	void turnBy(f32 degs, bool ccw);
	
	protected:
	
	private:
	IrrlichtDevice* Device;
	scene::ISceneNode* Node;
	
	u32 TurnStartTime;
	f32 TotalTurnAngleDegs;
	bool CCW;
	
	core::vector3df OldRot;
	
	bool IsTurning;
	
	void rotate(scene::ISceneNode* node, const core::vector3df& rot);
	core::vector3df getDesiredRot(const core::vector3df& oldRot, f32 turnAngleDegs, bool ccw);
	
};







#endif // _TURN_CONTROL_H_




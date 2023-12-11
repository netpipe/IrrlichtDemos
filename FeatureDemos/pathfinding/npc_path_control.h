#ifndef _NPC_PATH_CONTROL_H_
#define _NPC_PATH_CONTROL_H_

#include <vector>
#include <deque>
#include <irrlicht.h>

#include "catmull_rom.h"

using namespace irr;

#ifndef NPC_PATH_CONTROL_PROPERTIES
#define NPC_PATH_CONTROL_PROPERTIES
#define SPEED 1.0f
#endif



enum NpcPathControlState
{
	IDLE_STATE = 0,
	RUNNING_STATE,
	PAUSE_STATE,
	FINISHED_STATE
};

class NpcPathControl
{
	public:
	
	NpcPathControl
	(
		IrrlichtDevice* device,
		scene::ISceneNode* node
	);
	virtual ~NpcPathControl();

	static
	f32 getAnimatedMeshSceneNodeYOffset(scene::IAnimatedMeshSceneNode*  node);
	
	
	static
	void teleportTo
	(
		scene::ISceneNode* node,
		const core::vector3df& pos,
		f32 yoffset = 0.0f
	);
	/***
	static
	void updateFollowCamera
	(
		scene::ICameraSceneNode* camera,
		scene::ISceneNode* target,
		const core::vector3df& offset
	);
		CameraYawEmpty->setPosition(cameraOffset);
	
	this->updateAbsolutePosition();
	
	// target:
	camera->updateAbsolutePosition();
	core::vector3df camPos = camera->getAbsolutePosition();
	core::vector3df targetVector = camera->getTarget() - camPos;
	
	core::vector3df forward = core::vector3df(0.0f, 0.0f, 1.0f);
	//core::matrix4 m1 = PitchEmpty->getAbsoluteTransformation();
	core::matrix4 m1 = CameraPitchEmpty->getAbsoluteTransformation();
	m1.rotateVect(forward);
	forward.normalize();
	
	
	
	// pos:
	core::vector3df pos = RootEmpty->getAbsolutePosition();
	
	
	// up:
	core::vector3df upVector = core::vector3df(0.0f, 1.0f, 0.0f);
	//core::matrix4 m2 = RollEmpty->getAbsoluteTransformation();
	core::matrix4 m2 = CameraPitchEmpty->getAbsoluteTransformation();
	m2.rotateVect(upVector);
	upVector.normalize();
	
	core::vector3df offset = cameraOffset;
	m2.rotateVect(offset);

	// set cam prop.
	camera->setTarget(forward * targetVector.getLength() + pos + offset);
	camera->setPosition(pos + offset);
	camera->setUpVector(upVector);

	***/
	static
	void resetAnimatorCollisionResponse(scene::ISceneNode* node);

	void moveNodeTo(const core::vector3df& pos);
	
	virtual void start();
	virtual void pause();
	virtual void resume();
	virtual void stop();
	virtual void update();
	virtual void init();
	
	
	
	// return true to continue:
	virtual bool gotoWaypoint
	(
		const core::vector3df& waypoint,
		f32 speed,
		f32 dt,
		f32 ddsq = 20.0f // delta distance squared
	);
	
	
	
	virtual scene::ISceneNode* getNode() const { return Node; };
	
	
	void setWaypoints(const core::array<core::vector3df>& waypoints);
	
	void clear() { Waypoints.clear(); }
	void setSpeed(f32 speed) { Speed = speed; }
	void setYOffset(f32 offset) { YOffset = offset; }
	
	core::array<core::vector3df> getWaypoints() const { return Waypoints; }
	f32 getSpeed() const { return Speed; }
	f32 getYOffset() const { return YOffset; }
	
	std::deque<core::vector3df> SplinePoints; // for visual debug
	
	
	protected:
	
	private:
	IrrlichtDevice* Device;
	scene::ISceneNode* Node;
	CatmullRom* Spline;
	
	core::array<core::vector3df> Waypoints;
	
	
	u32 Time;
	u32 LastTime;
	
	
	f32 Speed;
	f32 YOffset;
	NpcPathControlState State;
	bool IsFirstRun;

	// for catmull_rom
	s32 P0, P1, P2, P3;
	u32 RunCount;// e.g. for 10 points, there are 9 segments to transverse: RunCount=9
	bool IsSplineStarted;
	bool IsSplineEnded;
	f32 SplineElapsedTime;
};







#endif // _NPC_PATH_CONTROL_H_


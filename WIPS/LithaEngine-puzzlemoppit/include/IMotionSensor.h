
#ifndef I_MOTION_SENSOR_H
#define I_MOTION_SENSOR_H

#include "ISensor.h"

class IMotionCallback : public virtual IReferenceCounted
{
public:
	virtual void OnMotionStart() = 0;
	virtual void OnMotionStop() = 0;
};

class IMotionSensor : public ISensor
{
public:
	
	// Called when translation occurs.
	virtual void SetTranslateMotionCallback(IMotionCallback *callback) = 0;
	
	// Called when rotation occurs.
	virtual void SetRotateMotionCallback(IMotionCallback *callback) = 0;
	
	// Called if either translation or rotation occurs.
	// This is called as well as the more specific translation or rotation callback.
	virtual void SetAnyMotionCallback(IMotionCallback *callback) = 0;
	
	// Detect only relative motion? (i.e. in this node's local space)
	// Otherwise defaults to absolute motion. (will detect motion due to parent nodes)
	// Changing this resets detection (so if initial events are enabled they will be resent).
	virtual void SetRelative(bool relative) = 0;
	
	// Should initial events be sent?
	// I.e. an OnMotionStart event if the object is already in motion when this sensor is created.
	// or an OnMotionStop if the object is initially stationary.
	// Defaults to true.
	// To have an effect must be called right after creation, before the next logic update.
	virtual void EnableInitialEvents(bool initialEvents) = 0;
	
	// Sets a time in seconds that must pass before a change in motion is detected.
	// Default is a 10th of a second (0.1).
	// Hmm, this needs thinking out. Perhaps there are better parameters.
	//virtual void SetWait(f32 wait) = 0;
	
	// Translational speed below which no motion is detected and object is considered stationary.
	// Default 0.0001 units/second.
	virtual void SetMinTranslateSpeed(f32 speed) = 0;
	
	// Rotational speed below which no motion is detected and object is considered stationary.
	// Default 0.001 degrees/second.
	virtual void SetMinRotateSpeed(f32 speed) = 0;
	
	// Motion is averaged over this number of previous logic steps.
	// Beware setting this too high; a list this size of previous positions and rotations
	// must be stored and averaged each update.
	// Default 0, averaging disabled.
	virtual void SetAveragingCount(u32 count) = 0;
	
	// Position/rotation for averaging are saved at this interval.
	// Default 0, meaning no steps are skipped, values recorded every update.
	// Increasing this can increase the time span encompassed without increasing the averaging count.
	virtual void SetAveragingSkip(u32 skip) = 0;
	
	// which translation axis to be considered?
	// default is all three x,y,z
	// this should only be called just after creation
	// (shouldn't change it while sensor is running)
	virtual void EnableTranslateAxes(s32 flags = AXIS_X|AXIS_Y|AXIS_Z) = 0;
};

#endif


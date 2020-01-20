
#include "IMotionSensor.h"
#include <vector>
#include "InfiniteRunningAverage.h"

// Note: This implementation has not really been tested.
class MotionSensor : public IMotionSensor
{
	IMotionCallback *translateCallback;
	IMotionCallback *rotateCallback;
	IMotionCallback *anyCallback;
	
	bool relative;
	bool initialEvents;
	
	f32 minTranslateSpeed;
	f32 minRotateSpeed;
	
	u32 averagingCount;
	u32 averagingSkip;
	
	// for counting skips...
	u32 skipCounter;
	
	// Counts the first two updates.
	// (cannot find a change in motion until after that)
	u8 updateCount;
	
	// maybe we don't consider all axes
	s32 translateAxesEnabled;
	
	// Cached values from previous update.
	
	core::vector3df lastPos;
	core::vector3df lastRot;
	
	std::vector<f32> posDeltaHistory;
	std::vector<f32> rotDeltaHistory;
	
	InfiniteRunningAverage posDeltaAverage;
	InfiniteRunningAverage rotDeltaAverage;
	
	bool wasTranslating;
	bool wasRotating;
	
	void ResetAveraging();
	
public:
	MotionSensor();
	~MotionSensor();
	
	void SetTranslateMotionCallback(IMotionCallback *callback);
	void SetRotateMotionCallback(IMotionCallback *callback);
	void SetAnyMotionCallback(IMotionCallback *callback);
	
	void SetRelative(bool relative);
	void EnableInitialEvents(bool initialEvents);
	
	void SetMinTranslateSpeed(f32 speed);
	void SetMinRotateSpeed(f32 speed);
	
	void SetAveragingCount(u32 count);
	void SetAveragingSkip(u32 skip);
	
	void EnableTranslateAxes(s32 flags);
	
	void Update(f32 dt);
};


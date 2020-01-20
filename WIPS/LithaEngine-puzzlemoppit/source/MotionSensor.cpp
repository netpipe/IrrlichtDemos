
#include "MotionSensor.h"

MotionSensor::MotionSensor()
{
	translateCallback = NULL;
	rotateCallback = NULL;
	anyCallback = NULL;
	
	updateCount = 0;
	skipCounter = 0;
	
	// Defaults
	SetRelative(false);
	EnableInitialEvents(true);
	SetMinTranslateSpeed(0.0001);
	SetMinRotateSpeed(0.001);
	SetAveragingCount(0);
	SetAveragingSkip(0);
	EnableTranslateAxes(AXIS_X|AXIS_Y|AXIS_Z);
	
	// Init
	// (though not really needed as may be called by Set* function calls above)
	ResetAveraging();
}

MotionSensor::~MotionSensor()
{
	if (translateCallback)
		translateCallback->drop();
	
	if (rotateCallback)
		rotateCallback->drop();
	
	if (anyCallback)
		anyCallback->drop();
}

void MotionSensor::ResetAveraging()
{
	posDeltaHistory.clear();
	rotDeltaHistory.clear();
	
	// Forces a history value to be cached on next update.
	skipCounter = averagingSkip;
}

void MotionSensor::SetTranslateMotionCallback(IMotionCallback *callback)
{
	SET_REF_COUNTED_POINTER(translateCallback, callback)
}

void MotionSensor::SetRotateMotionCallback(IMotionCallback *callback)
{
	SET_REF_COUNTED_POINTER(rotateCallback, callback)
}

void MotionSensor::SetAnyMotionCallback(IMotionCallback *callback)
{
	SET_REF_COUNTED_POINTER(anyCallback, callback)
}

void MotionSensor::SetRelative(bool relative)
{
	// If it is changed, we want to start motion detecting again.
	if (this->relative != relative)
	{
		updateCount = 0;
		ResetAveraging();
	}
	
	this->relative = relative;
}

void MotionSensor::EnableInitialEvents(bool initialEvents)
{
	this->initialEvents = initialEvents;
}

void MotionSensor::SetMinTranslateSpeed(f32 speed)
{
	minTranslateSpeed = speed;
}

void MotionSensor::SetMinRotateSpeed(f32 speed)
{
	minRotateSpeed = speed;
}

void MotionSensor::SetAveragingCount(u32 count)
{
	averagingCount = count;
	
	ResetAveraging();
}

void MotionSensor::SetAveragingSkip(u32 skip)
{
	averagingSkip = skip;
	
	// And also reset averaging, so new values are found with the new skip.
	ResetAveraging();
}

void MotionSensor::EnableTranslateAxes(s32 flags)
{
	translateAxesEnabled = flags;
}

void MotionSensor::Update(f32 dt)
{
	core::vector3df pos;
	core::vector3df rot;
	
	if (relative)
	{
		pos = GetPosition();
		rot = GetRotation();
	}
	else
	{
		pos = GetAbsolutePosition();
		rot = GetAbsoluteRotation();
	}
	
	if (updateCount == 0)
	{
		updateCount ++;
		// Do nothing on first update.
		// lastPos and lastRot are cached below.
	}
	else
	{
		// Now have at least one previous frame, so can cache deltas in history.
		
		// Only perform averaging every averageSkip number of updates
		// But MUST be done on first update.
		// We want at least one set of values Add()ed to averages before calling Get().
		if (skipCounter >= averagingSkip)
		{
			skipCounter = 0;
			
			f32 translateDist;
			
			// for translation, we may only consider distances along some of the axes.
			if ( translateAxesEnabled == (AXIS_X|AXIS_Y|AXIS_Z) )
				translateDist = pos.getDistanceFrom(lastPos);
			else if ( translateAxesEnabled == (AXIS_X|AXIS_Y) )
				translateDist = core::vector2df(pos.X,pos.Y).getDistanceFrom(core::vector2df(lastPos.X,lastPos.Y));
			else if ( translateAxesEnabled == (AXIS_X|AXIS_Z) )
				translateDist = core::vector2df(pos.X,pos.Z).getDistanceFrom(core::vector2df(lastPos.X,lastPos.Z));
			else if ( translateAxesEnabled == (AXIS_Z|AXIS_Y) )
				translateDist = core::vector2df(pos.Z,pos.Y).getDistanceFrom(core::vector2df(lastPos.Z,lastPos.Y));
			else
				translateDist = 0.f;
			
			// We want average *deltas*, not the elements themselves.
			// So we want to store the deltas each frame?
			posDeltaHistory.push_back( translateDist );
			rotDeltaHistory.push_back( maths::scalar_rotation_difference(rot, lastRot) );
			
			// erase oldest history element
			if (posDeltaHistory.size() > averagingCount + 1) // +1, want at least one element
			{
				posDeltaHistory.erase(posDeltaHistory.begin());
				rotDeltaHistory.erase(rotDeltaHistory.begin());
			}
			
			// Averaging
			
			posDeltaAverage.Reset();
			rotDeltaAverage.Reset();
			
			for (u32 i = 0; i < posDeltaHistory.size(); i ++)
			{
				posDeltaAverage.Add(posDeltaHistory[i]);
				rotDeltaAverage.Add(rotDeltaHistory[i]);
			}
		}
		else
		{
			skipCounter ++;
		}
		
		// At least one history value should have been added by now.
		ASSERT(posDeltaAverage.Valid() && rotDeltaAverage.Valid());
		ASSERT(posDeltaHistory.size() && rotDeltaHistory.size());
		
		// Don't actually start logic until history has reached the desired size
		// (averagingCount + 1)
		if (posDeltaHistory.size() > averagingCount)
		{
			ASSERT(posDeltaHistory.size() == averagingCount + 1);
			ASSERT(posDeltaHistory.size() == rotDeltaHistory.size());
			
			// Assumption: dt is the same each update. (run at fixed timestep)
			bool isTranslating = posDeltaAverage.Get() >= dt * minTranslateSpeed;
			bool isRotating = rotDeltaAverage.Get() >= dt * minRotateSpeed;
			
			// And then as before...
			// First update with valid history.
			if (updateCount == 1)
			{
				updateCount ++;
				
				// Now wasTranslating and wasRotating will be cached to valid
				// values below. (due to lastPos/lastRot being valid above)
				
				// We now have one previous frame, so can tell if we are in motion!
				// Perhaps send initial events.
				
				if (initialEvents)
				{
					if (translateCallback)
					{
						if (isTranslating)
							translateCallback->OnMotionStart();
						else
							translateCallback->OnMotionStop();
					}
					
					if (rotateCallback)
					{
						if (isRotating)
							rotateCallback->OnMotionStart();
						else
							rotateCallback->OnMotionStop();
					}
					
					if (anyCallback)
					{
						if (isRotating || isTranslating)
							anyCallback->OnMotionStart();
						else
							anyCallback->OnMotionStop();
					}
				}
			}
			// Subsequent updates
			else
			{
				// Now have at least two previous frames, so we can detect if a change in motion has occurred.
				
				if (translateCallback)
				{
					if (isTranslating && !wasTranslating)
						translateCallback->OnMotionStart();
					
					if (!isTranslating && wasTranslating)
						translateCallback->OnMotionStop();
				}
				
				if (rotateCallback)
				{
					if (isRotating && !wasRotating)
						rotateCallback->OnMotionStart();
					
					if (!isRotating && wasRotating)
						rotateCallback->OnMotionStop();
				}
				
				if (anyCallback)
				{
					bool isMotion = isTranslating || isRotating;
					bool wasMotion = wasTranslating || wasRotating;
					
					if (isMotion && !wasMotion)
						anyCallback->OnMotionStart();
					
					if (!isMotion && wasMotion)
						anyCallback->OnMotionStop();
				}
			}
			
			// Cache values for next frame.
			wasTranslating = isTranslating;
			wasRotating = isRotating;
		}
	}
	
	// Cache values for next frame.
	lastPos = pos;
	lastRot = rot;
}

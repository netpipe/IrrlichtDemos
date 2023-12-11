
#include "IMotionSensor.h"
#include "ISoundSource.h"
#include "ISoundQueue.h"

class SoundMotionCallback : public IMotionCallback
{
	core::stringc startSound;
	core::stringc stopSound;
	core::stringc motionLoopSound;
	
	ISoundQueue *soundQueue;
	
public:
	// all sounds should be optional.
	SoundMotionCallback(ISoundSource *soundSource, const c8 *startSound, const c8 *stopSound, const c8 *motionLoopSound)
	{
		this->startSound = startSound;
		this->stopSound = stopSound;
		this->motionLoopSound = motionLoopSound;
		
		soundQueue = soundSource->GetSoundQueue();
	}
	
	void OnMotionStart()
	{
		if (startSound.size())
			soundQueue->Add3D(startSound.c_str(), ESF_ENQUEUE|ESF_NO_WAIT);
		
		if (motionLoopSound.size())
			soundQueue->Add3D(motionLoopSound.c_str(), ESF_ENQUEUE|ESF_LOOP|ESF_UNTIL_NEXT);
	}
	
	void OnMotionStop()
	{
		if (stopSound.size())
			soundQueue->Add3D(stopSound.c_str(), ESF_ENQUEUE|ESF_NO_WAIT);
		else
			soundQueue->BreakLoop();
	}
};



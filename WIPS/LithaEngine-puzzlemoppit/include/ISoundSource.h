
#ifndef I_SOUND_SOURCE
#define I_SOUND_SOURCE

#include "ITransformable.h"
#include "IPausable.h"

class ISound3D;
class ISoundQueue;

class ISoundSource : public ITransformable, public IPausable
{
public:
	virtual ~ISoundSource(){}
	
	// Get a single sound that can be played.
	// If you want to play more than one sound at the same time, use AddSound()
	// The sound has its position, velocity, etc set internally by the sound source.
	virtual ISound3D *GetSound() = 0;
	
	// A more advanced way of playing sounds.
	// The multiple ISounds of the sound queue have their position, velocity etc set by the sound source.
	virtual ISoundQueue *GetSoundQueue() = 0;
	
	// Add a new sound to this source.
	// If deleteOnFinish is set, then the sound will be removed after it has been played once.
	//virtual ISound3D *AddSound(bool deleteOnFinish = true) = 0;
	
	// Called internally. Updates position, velocity etc of sounds.
	virtual void Update(f32 dt) = 0;
};

#endif

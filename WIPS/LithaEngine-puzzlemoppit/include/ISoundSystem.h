
#ifndef I_SOUND_SYSTEM_H
#define I_SOUND_SYSTEM_H

#include "litha_internal.h"

class ISound2D;
class ISound3D;
class ISoundQueue;

// Sound system.
class ISoundSystem : public virtual IReferenceCounted
{
public:
	virtual ~ISoundSystem(){}
	
	// Get an interface for playing and controlling playback of a single sound.
	virtual ISound2D *CreateSound2D() = 0;
	virtual ISound3D *CreateSound3D() = 0;
	
	// An interface for queuing sounds to play one after another.
	virtual ISoundQueue *CreateSoundQueue() = 0;
	
	// Preload a sound. So when later calling ISound.Play for this, there shouldn't be any delay.
	virtual void PreloadSound(const c8 *soundFile) = 0;
	
	// Used internally to set the listener position.
	virtual void SetListenerPosition(core::vector3df pos) = 0;
	
	// Used internally to set the listener orientation.
	virtual void SetListenerOrientation(core::vector3df lookVec, core::vector3df upVec = core::vector3df(0,1,0)) = 0;
	
	// Used internally for doppler effect.
	virtual void SetListenerVelocity(core::vector3df vel) = 0;
	
	// Set the global volume. Affects all sounds.
	// Default 1.0
	virtual void SetGlobalVolume(f32 volume) = 0;
	
	// Seperate volume for all 2D sounds.
	virtual void SetGlobalVolume2D(f32 volume) = 0;
	
	// Seperate volume for all 3D sounds.
	virtual void SetGlobalVolume3D(f32 volume) = 0;
	
	// Stops everything currently playing.
	virtual void StopAllSounds() = 0;
};

#endif

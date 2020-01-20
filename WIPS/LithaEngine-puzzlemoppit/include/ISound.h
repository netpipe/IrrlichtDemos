
#ifndef I_SOUND_H
#define I_SOUND_H

#include "litha_internal.h"
#include "IPausable.h"

class ISound : public virtual IReferenceCounted, public IPausable
{
public:
	virtual ~ISound(){}
	
	// Plays the specified sound file, starting at the beginning.
	// Note that this does not clear the pause state; if you previously paused the sound
	// you will need to call Resume before it will begin playing.
	virtual void Play(const c8 *soundFile) = 0;
	
	// End playback of the sound.
	// Has no effect if the sound is not playing.
	// Does not clear the pause state.
	virtual void Stop() = 0;
	
	// whether the sound loops. If this is set false while the sound is already playing, it will finish
	// at the end of the current play.
	// This state is remembered across calls to Play(..). (so can be called before a call to Play)
	// Default state is not looped.
	virtual void SetIsLooped(bool loop) = 0;
	
	// Returns true if the sound has played to the end or if the sound has had Stop() called.
	virtual bool IsFinished() = 0;
	
	// Is it currently playing?
	virtual bool IsPlaying() = 0;
	
	// Gets the current play position within the sound in seconds.
	// Returns 0 if no sound has been played or if the sound has finished.
	virtual f32 GetPlayPosition() = 0;
	
	// Set the volume! 0 is silent, 1 is full.
	// Default is 1.0, full volume.
	virtual void SetVolume(f32 volume) = 0;
};

#endif

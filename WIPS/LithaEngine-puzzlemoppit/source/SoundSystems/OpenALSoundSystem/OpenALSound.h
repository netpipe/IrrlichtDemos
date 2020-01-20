
#ifndef OPENAL_SOUND_H
#define OPENAL_SOUND_H

#include "ISound.h"
#include "openal_stuff.h"

class OpenALSoundSystem;

class OpenALSound : public virtual ISound
{
	f32 volume;
	
	// this "separate" volume is used by OpenALSoundSystem to provide separate volume
	// settings for 2D and 3D sounds. So it is used as a multiplier within the ApplyVolume call.
	f32 separateVolume;
	
protected:
	OpenALSoundSystem *soundSystem;
	
	ALuint source;
	
	void OnPause();
	void OnResume();
	
public:
	OpenALSound(OpenALSoundSystem *soundSystem);
	~OpenALSound();
	
	void Play(const c8 *soundFile);
	
	void Stop();
	void SetIsLooped(bool loop);
	bool IsFinished();
	bool IsPlaying();
	f32 GetPlayPosition();
	void SetVolume(f32 volume);
	
	void SetSeparateVolume(f32 volume);
	
	// actually set the openal volume, taking into account
	// both the regular volume and the separate volume.
	void ApplyVolume();
};

#endif

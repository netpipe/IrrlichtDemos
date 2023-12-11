
#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include "ISoundSource.h"

class ISoundSystem;

class SoundSource : public ISoundSource
{
	ISoundSystem *soundSystem;
	ISound3D *sound;
	ISoundQueue *soundQueue;
	
protected:
	void OnPause();
	void OnResume();
	
public:
	SoundSource(ISoundSystem *soundSystem);
	~SoundSource();
	
	ISound3D *GetSound();
	ISoundQueue *GetSoundQueue();
	
	void Update(f32 dt);
};

#endif


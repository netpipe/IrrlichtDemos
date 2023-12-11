
#include "SoundSystem.h"
#include "SoundQueue.h"

ISoundQueue *SoundSystem::CreateSoundQueue()
{
	return new SoundQueue(this);
}



#include "ISoundSystem.h"

class SoundSystem : public ISoundSystem
{
public:
	// Sound queue makes use of ISoundSystem and does not need to know about the lower level sound API.
	ISoundQueue *CreateSoundQueue();
};

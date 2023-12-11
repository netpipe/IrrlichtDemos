
#include "ISound3D.h"
#include "OpenALSound.h"

class OpenALSound3D : public OpenALSound, public ISound3D
{
public:
	OpenALSound3D(OpenALSoundSystem *soundSystem);
	~OpenALSound3D();
	
	void SetPosition(core::vector3df pos);
	void SetVelocity(core::vector3df vel);
	void SetMaxVolumePoint(f32 proximity);
};

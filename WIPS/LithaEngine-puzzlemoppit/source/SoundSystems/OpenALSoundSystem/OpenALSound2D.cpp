
#include "OpenALSound2D.h"
#include "sound_reg.h"

OpenALSound2D::OpenALSound2D(OpenALSoundSystem *soundSystem)
	: OpenALSound(soundSystem)
{
	// Not 3D sound.
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	check_openal_error();
	
	SetPan(0.0);
	
	register_sound2d(this);
}

OpenALSound2D::~OpenALSound2D()
{
	unregister_sound2d(this);
}

void OpenALSound2D::SetPan(f32 panning)
{
	// This probably won't work as desired (i.e. to mimick IrrKlang) - not tested.
	// need AL_GAIN_LINEAR??
	//http://www.torquepowered.com/community/blogs/view/9701
	alSource3f(source, AL_POSITION, panning,0,0);
	check_openal_error();
}



#include "OpenALSound3D.h"
#include "sound_reg.h"

OpenALSound3D::OpenALSound3D(OpenALSoundSystem *soundSystem)
	: OpenALSound(soundSystem)
{
	SetPosition( core::vector3df(0,0,0) );
	SetVelocity( core::vector3df(0,0,0) );
	SetMaxVolumePoint( 1.0 );
	
	register_sound3d(this);
}

OpenALSound3D::~OpenALSound3D()
{
	unregister_sound3d(this);
}

void OpenALSound3D::SetPosition(core::vector3df pos)
{
	alSource3f(source, AL_POSITION, pos.X,pos.Y,-pos.Z);
	check_openal_error();
}

void OpenALSound3D::SetVelocity(core::vector3df vel)
{
	alSource3f(source, AL_VELOCITY, vel.X,vel.Y,-vel.Z);
	check_openal_error();
}

void OpenALSound3D::SetMaxVolumePoint(f32 proximity)
{
	// AL_REFERENCE_DISTANCE may not entirely be correct.
	// it may vary depending on other OpenAL settings.
	// Consider it untested.
	alSourcef(source, AL_REFERENCE_DISTANCE, proximity);
	check_openal_error();
}





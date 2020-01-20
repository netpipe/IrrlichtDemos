
#include "OpenALSound.h"
#include "OpenALSoundSystem.h"

s32 sourceCounter = 0;

OpenALSound::OpenALSound(OpenALSoundSystem *soundSystem)
{
	this->soundSystem = soundSystem;
	
	volume = 1.f;
	separateVolume = 1.f;
	
	source = 0;
	alGenSources(1, &source);
	
	if (check_openal_error())
	{
		WARN << "OpenAL source count probably exceeded: " << sourceCounter << " sources in use.";
	}
	
	SetIsLooped(false);
	ApplyVolume();
	
	sourceCounter ++;
}

OpenALSound::~OpenALSound()
{
	Stop();
	
	alDeleteSources(1, &source);
	check_openal_error();
	
	sourceCounter --;
}

void OpenALSound::Play(const c8 *soundFile)
{
	Stop();
	
	// Get the OpenAL buffer for this sound file.
	
	ALuint buffer;
	
	if (!soundSystem->GetOpenALBuffer(soundFile, &buffer))
	{
		WARN << "Could not get buffer (" << soundFile << ")";
		return;
	}
	
	// If this buffer is not already set, then set it.
	
	ALint currentBuffer;
	alGetSourcei(source, AL_BUFFER, &currentBuffer);
	
	if (check_openal_error())
		return;
	
	if ((ALint)buffer != currentBuffer)
	{
		alSourcei(source, AL_BUFFER, buffer);
		
		if (check_openal_error())
			return;
	}
	
	// Start playing
	
	if (!IsPaused())
	{
		alSourcePlay(source);
		check_openal_error();
	}
	
	// OpenAL source retains settings like looping, volume, so we don't need to re-set them here.
}

void OpenALSound::Stop()
{
	alSourceStop(source);
	check_openal_error();
}

void OpenALSound::OnPause()
{
	if (!IsFinished())
	{
		alSourcePause(source);
		check_openal_error();
	}
}

void OpenALSound::OnResume()
{
	if (!IsFinished())
	{
		alSourcePlay(source);
		check_openal_error();
	}
}

void OpenALSound::SetIsLooped(bool loop)
{
	alSourcei(source, AL_LOOPING, loop);
	check_openal_error();
}

bool OpenALSound::IsFinished()
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	
	if (check_openal_error())
		return false;
	
	return state == AL_STOPPED;
}

bool OpenALSound::IsPlaying()
{
	ALint state;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	
	if (check_openal_error())
		return false;
	
	return state == AL_PLAYING;
}

f32 OpenALSound::GetPlayPosition()
{
	if (IsFinished())
		return 0.0;
	
    ALfloat playPosition = 0.0;
    alGetSourcef(source, AL_SEC_OFFSET, &playPosition);
    
    if (check_openal_error())
		return 0.0;
	
	return playPosition;
}

void OpenALSound::SetVolume(f32 volume)
{
	this->volume = volume;
	ApplyVolume();
}

void OpenALSound::SetSeparateVolume(f32 volume)
{
	separateVolume = volume;
	ApplyVolume();
}

void OpenALSound::ApplyVolume()
{
	alSourcef(source, AL_GAIN, volume*separateVolume);
	check_openal_error();
}

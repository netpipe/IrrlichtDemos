
#include "OpenALSoundSystem.h"
#include "OpenALSound2D.h"
#include "OpenALSound3D.h"
#include "stb_vorbis.h"
#include "sound_reg.h"

OpenALSoundSystem::OpenALSoundSystem()
{
	device = alcOpenDevice(NULL);
	context = NULL;
	
	if (!device)
	{
		WARN << "Failed to create OpenAL device.";
	}
	else
	{
		context = alcCreateContext(device, NULL);
		
		if (!context)
		{
			alcCloseDevice(device);
			device = NULL;
			WARN << "Failed to create OpenAL context.";
		}
		else
		{
			if (!alcMakeContextCurrent(context))
			{
				alcDestroyContext(context);
				alcCloseDevice(device);
				context = NULL;
				device = NULL;
				WARN << "Failed to make OpenAL context current.";
			}
		}
	}
	
	globalVolume2D = 1.f;
	globalVolume3D = 1.f;
	
	SetGlobalVolume(1.0);
	SetListenerPosition(core::vector3df(0,0,0));
	SetListenerOrientation( core::vector3df(0,0,1), core::vector3df(0,1,0) );
	SetListenerVelocity(core::vector3df(0,0,0));
}

OpenALSoundSystem::~OpenALSoundSystem()
{
	// delete all OpenAL buffers
	for (std::map<core::stringc, ALuint>::const_iterator i = buffers.begin(); i != buffers.end(); i ++)
	{
		alDeleteBuffers(1, &i->second);
		check_openal_error();
	}
	
	if (device && context)
	{
		check_openal_error();
		
		if (!alcMakeContextCurrent(NULL))
			WARN << "Failed to clear current OpenAL context.";
		else
		{
			alcDestroyContext(context);
			check_openal_error();
			
			if (!alcCloseDevice(device))
				WARN << "Could not close OpenAL device.";
		}
	}
}

bool OpenALSoundSystem::GetOpenALBuffer(const core::stringc &fileName, ALuint *buffer)
{
	// Does buffer already exist?
	if (buffers.count(fileName))
	{
		*buffer = buffers[fileName];
		return true;
	}
	
	// Otherwise we create buffer.
	
	// determine file type by extension
	io::path fileExt = os::path::getext(fileName);
	
	if (fileExt == "wav")
	{
		// Load WAV files with ALUT.
		
		return false;
		
		/*
		ALuint newbuffer = alutCreateBufferFromFile(fileName.c_str());
		
		if (newbuffer == AL_NONE)
		{
			WARN << "Could not create buffer from file (" << fileName << ")";
			check_openal_error();
		}
		else
		{
			buffers[fileName] = newbuffer;
			*buffer = newbuffer;
			return true;
		}
		*/
	}
	else if (fileExt == "ogg")
	{
		// Try the Ogg loader.
		// In future, ogg files should probably be streamed rather than entirely buffered in memory.
		
		ALuint newbuffer = 0;
		alGenBuffers(1, &newbuffer);
		
		if (check_openal_error())
			return false;
		else
		{
			// First query to find frequency. (since the other function doesn't give it)
			
			ALsizei frequency;
			bool gotFrequency = false;
			
			int n, error;
			stb_vorbis *v = stb_vorbis_open_filename((char *)fileName.c_str(), &error, NULL);
			
			if (v)
			{
				stb_vorbis_info info = stb_vorbis_get_info(v);
				
				frequency = info.sample_rate;
				
				stb_vorbis_close(v);
				
				gotFrequency = true;
			}
			
			
			// Load the ogg
			
			short *data = NULL;
			int channels, len;
			
			len = stb_vorbis_decode_filename((char *)fileName.c_str(), &channels, &data);
			
			if (len && gotFrequency)
			{
				// Fill buffer with loaded ogg file.
				
				if (channels == 1 || channels == 2)
				{
					ALenum format;
					
					if (channels == 1)
						format = AL_FORMAT_MONO16;
					else
						format = AL_FORMAT_STEREO16;
					
					alBufferData(newbuffer, format, data, sizeof(short)*len*channels, frequency);
					
					if (!check_openal_error())
					{
						if (data)
							free(data);
						
						buffers[fileName] = newbuffer;
						*buffer = newbuffer;
						return true;
					}
					else
					{
						WARN << "alBufferData failed for Ogg file, " << fileName;
					}
				}
				else
				{
					WARN << "Ogg file has invalid number of channels: (" << channels << ") - " << fileName;
				}
			}
			else
			{
				WARN << "Ogg loading failed: " << fileName;
			}
			
			if (data)
				free(data);
			
			alDeleteBuffers(1, &newbuffer);
			check_openal_error(); // may not be an openal error if it was ogg loader that failed
			
			return false;
		}
	}
	
	return false;
}

void OpenALSoundSystem::PreloadSound(const c8 *soundFile)
{
	ALuint buffer;
	
	if (!GetOpenALBuffer(soundFile, &buffer))
		WARN << "Could not get buffer (" << soundFile << ")";
}

void OpenALSoundSystem::SetListenerPosition(core::vector3df pos)
{
	alListener3f(AL_POSITION, pos.X,pos.Y,-pos.Z);
	check_openal_error();
}

void OpenALSoundSystem::SetListenerOrientation(core::vector3df lookVec, core::vector3df upVec)
{
	ALfloat orientation[6];
	orientation[0] = lookVec.X;
	orientation[1] = lookVec.Y;
	orientation[2] = -lookVec.Z;
	orientation[3] = upVec.X;
	orientation[4] = upVec.Y;
	orientation[5] = -upVec.Z;
	
	alListenerfv(AL_ORIENTATION, orientation);
	check_openal_error();
}

void OpenALSoundSystem::SetListenerVelocity(core::vector3df vel)
{
	alListener3f(AL_VELOCITY, vel.X,vel.Y,-vel.Z);
	check_openal_error();
}

ISound2D *OpenALSoundSystem::CreateSound2D()
{
	OpenALSound2D *newSound = new OpenALSound2D(this);
	newSound->SetSeparateVolume(globalVolume2D);
	return newSound;
}

ISound3D *OpenALSoundSystem::CreateSound3D()
{
	OpenALSound3D *newSound = new OpenALSound3D(this);
	newSound->SetSeparateVolume(globalVolume3D);
	return newSound;
}

void OpenALSoundSystem::SetGlobalVolume(f32 volume)
{
	alListenerf(AL_GAIN, volume);
	check_openal_error();
}

void OpenALSoundSystem::SetGlobalVolume2D(f32 volume)
{
	globalVolume2D = volume;
	
	for (u32 i = 0; i < allSounds2D.size(); i ++)
		allSounds2D[i]->SetSeparateVolume(volume);
}

void OpenALSoundSystem::SetGlobalVolume3D(f32 volume)
{
	globalVolume3D = volume;
	
	for (u32 i = 0; i < allSounds3D.size(); i ++)
		allSounds3D[i]->SetSeparateVolume(volume);
}

void OpenALSoundSystem::StopAllSounds()
{
	for (u32 i = 0; i < allSounds.size(); i ++)
		allSounds[i]->Stop();
}


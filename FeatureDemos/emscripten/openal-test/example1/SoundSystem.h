
// Date: 28/07/2004
// By: Lukas Heise

// ###############################################################################

// Usage Instructions
// ==================
// SoundSystem soundSystem;
// tSound mySound = soundSystem.Load("soundfile.wav");
// soundSystem.Play(mySound);
//
// when finished using the sound call:
// soundSystem.Remove(mySound);

// ###############################################################################

#ifndef __SOUNDSYSTEM_H
#define __SOUNDSYSTEM_H


#include <stdio.h>
#include <vector>

#include <AL/alut.h>
#include <AL/alc.h>
#include <AL/al.h>
#include <AL/alu.h>
#include <Ogg/ogg.h>
#include <Vorbis/codec.h>
#include <Vorbis/vorbisenc.h>
#include <Vorbis/vorbisfile.h>

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "Alut.lib")
#pragma comment(lib, "ogg_static.lib")
#pragma comment(lib, "vorbis_static.lib")
#pragma comment(lib, "vorbisenc_static.lib")
#pragma comment(lib, "vorbisfile_static.lib")

#define OGG_BUFFER_SIZE		8192

using namespace std;



struct tSound
{
	unsigned int buffer;
	unsigned int source;
};


class SoundSystem
{
private:
	tSound LoadWAV(const char* strFile)
	{
		tSound ret = {0};
		ALenum format;
		ALvoid* data;
		ALsizei size;
		ALsizei freq;
		ALboolean loop;

		alGenBuffers(1,&ret.buffer);
		alGenSources(1,&ret.source);

		if (alGetError() != AL_NO_ERROR) return ret;

		alutLoadWAVFile((char*)strFile,&format,&data,&size,&freq,&loop);
		alBufferData(ret.buffer,format,data,size,freq);
		alutUnloadWAV(format,data,size,freq);

		alSourcei(ret.source,AL_BUFFER,ret.buffer);
		SetPitch(ret,1.0f);
		SetGain(ret,1.0f);
		SetLooping(ret,false);

		return ret;
	}

	tSound LoadOGG(const char* strFile)
	{
		tSound ret = {0};
		long bytes;
		vector<char> buffer;
		int bitStream;
		char array[OGG_BUFFER_SIZE] = {0};
		OggVorbis_File oggStream;
		vorbis_info* vorbisInfo;
		ALenum format;
		ALsizei freq;

		buffer.clear();
		FILE* fp = fopen(strFile,"rb");
		if (fp == NULL) return ret;
		

		alGenBuffers(1,&ret.buffer);
		alGenSources(1,&ret.source);

		if (alGetError() != AL_NO_ERROR) { fclose(fp); return ret; }

		if (ov_open(fp,&oggStream,NULL,0) != 0)
		{
			fclose(fp);
			return ret;
		}

		vorbisInfo = ov_info(&oggStream,-1);
		format = (vorbisInfo->channels == 1)? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		freq = vorbisInfo->rate;
	
		do
		{
			bytes = ov_read(&oggStream,array,OGG_BUFFER_SIZE,0,2,1,&bitStream);
			buffer.insert(buffer.end(),array,array + bytes);
		} while (bytes > 0);

		ov_clear(&oggStream);
		fclose(fp);

		alBufferData(ret.buffer,format,&buffer[0],static_cast<ALsizei>(buffer.size()),freq);

		buffer.clear();

		alSourcei(ret.source,AL_BUFFER,ret.buffer);
		SetPitch(ret,1.0f);
		SetGain(ret,1.0f);
		SetLooping(ret,false);

		return ret;
	}

public:
	SoundSystem()
	{
		alutInit(NULL,0);
	}

	virtual ~SoundSystem()
	{
		alutExit();
	}

	tSound Load(const char* strFile)
	{
		static tSound dummy = {0};
		char* type = strchr(strFile,'.');
		type++;

		     if (strcmpi(type,"wav") == 0) return LoadWAV(strFile);
		else if (strcmpi(type,"ogg") == 0) return LoadOGG(strFile);

		return dummy;
	}

	void Remove(tSound a)					{ Stop(a); alDeleteSources(1,&a.source); alDeleteBuffers(1,&a.buffer); }
	void Play(tSound id)					{ alSourcePlay(id.source); }
	void Stop(tSound id)					{ alSourceStop(id.source); }
	void Pause(tSound id)					{ alSourcePause(id.source); }
	void UnPause(tSound id)					{ Play(id); }
	bool IsPlaying(tSound id)				{ ALenum state; alGetSourcei(id.source,AL_SOURCE_STATE,&state); return (state == AL_PLAYING); }
	void SetLooping(tSound id, bool doLoop)	{ alSourcei(id.source,AL_LOOPING,doLoop); }
	void SetPosition(tSound id, float* pos)	{ alSourcefv(id.source,AL_POSITION,pos); }
	void SetVelocity(tSound id, float* vel)	{ alSourcefv(id.source,AL_VELOCITY,vel); }
	void SetPitch(tSound id, float pitch)	{ alSourcef(id.source,AL_PITCH,pitch); }
	void SetGain(tSound id, float gain)		{ alSourcef(id.source,AL_GAIN,gain); }
	void SetMaxDistance(tSound id, float d)	{ alSourcef(id.source,AL_MAX_DISTANCE,d); }
	void SetListenerPosition(float* pos)	{ alListenerfv(AL_POSITION,pos); }
	void SetListenerVelocity(float* vel)	{ alListenerfv(AL_VELOCITY,vel); }
	
	void SetListenerOrientation(float* at, float* up)
	{
		float ov[6] = { at[0], at[1], at[2], up[0], up[1], up[2] };
		alListenerfv(AL_ORIENTATION,ov);
	}

	void SetDopplerParameters(float factor, float velocity)
	{
		alDopplerFactor(factor);
		alDopplerVelocity(velocity);
	}
};

#endif


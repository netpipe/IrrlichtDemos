#include "Sound.h"
#include <al/alut.c>
//===========================================//
ALboolean INIT::InitOpenAL()
{
	ALCdevice* pDevice;
	ALCcontext* pContext;
	ALCubyte* defaultDevice;
	pDevice = alcOpenDevice((ALCubyte*)"DirectSound3D");
	pContext = alcCreateContext(pDevice, NULL);
	alcMakeContextCurrent(pContext);
	if(alcGetError(pDevice) != ALC_NO_ERROR)
		return AL_FALSE;
	alGetError(); 
	return AL_TRUE;
}
//===========================================//
Sound::Sound(){}
//===========================================//
Sound::Sound(ALbyte *File_Name)
{
	Name=File_Name;
	SourcePos[0]=0.0;    SourcePos[1]=0.0;  SourcePos[2]=0.0;
	SourceVel[0]=0.0;    SourceVel[1]=0.0;  SourceVel[2]=10.0;
	ListenerPos[0]=0.0;  ListenerPos[1]=0.0;   ListenerPos[2]=0.0;
	ListenerOri[0]=0.0;  ListenerOri[1]=0.0;  ListenerOri[2]=-1.0;
	ListenerVel[0]=0.0;  ListenerVel[1]=0.0;  ListenerVel[2]=0.0;
	//.....................................//
	LoadALData();
	SetListenerPos(0,0,0);
	SetListenerVelo(0,0,0);
	AddSource();

}
//===========================================//
void Sound::AddSource()
{
	alGenSources(1, &Source);

	if(alGetError() != AL_NO_ERROR)
	{
		printf("Error generating audio source.");
		exit(-1);
	}

	alSourcei (Source, AL_BUFFER,   Buffer);
	alSourcef (Source, AL_PITCH,    1.0f         );
	alSourcef (Source, AL_GAIN,     1.0f         );
	alSourcei (Source, AL_LOOPING,  AL_TRUE      );
}
//===========================================//
ALboolean Sound::LoadALData()
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;
	alGenBuffers(1, &Buffer);
	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;
	alutLoadWAVFile(Name, &format, &data, &size, &freq, &loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;
	return AL_TRUE;
}
//===========================================//
void Sound::SetListenerPos(ALfloat x , ALfloat y , ALfloat z)
{
	ListenerPos[0]=x;
	ListenerPos[1]=y;
	ListenerPos[2]=z;
	alListenerfv(AL_POSITION,ListenerPos);
}
//===========================================//
void Sound::SetListenerVelo(ALfloat x , ALfloat y , ALfloat z)
{
  
	ListenerVel[0]=x;
	ListenerVel[1]=y;
	ListenerVel[2]=z;
	alListenerfv(AL_VELOCITY,ListenerVel);
}
//===========================================//
void Sound::MovePos(ALfloat x, ALfloat y ,ALfloat z)
{
	SourcePos[0] = x;
	SourcePos[1] = y;
	SourcePos[2] = z;
	alSourcefv(Source, AL_POSITION, SourcePos );
}
//===========================================//
void Sound::MoveVelo(ALfloat x, ALfloat y ,ALfloat z)
{
	SourceVel[0] = x;
	SourceVel[1] = y;
	SourceVel[2] = z;
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSource3f(Source, AL_DIRECTION,0,0,1);
}
//===========================================//
void Sound::Play()
{
	alSourcePlay(Source);
}
//===========================================//
void Sound::Stop()
{
	alSourceStop(Source);
}
//===========================================//
void Sound::ExitOpenAL()
{
	ALCcontext* pCurContext;
	ALCdevice* pCurDevice;
	pCurContext = alcGetCurrentContext();
	pCurDevice = alcGetContextsDevice(pCurContext);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(pCurContext);
	alcCloseDevice(pCurDevice);
}
//===========================================//
void Sound::KillALData()
{
	alDeleteSources(1, &Source);
	alDeleteBuffers(1, &Buffer);
	ExitOpenAL();
}
void Sound::changeIntinsity(ALfloat t){

	//alSourcef (Source,AL_GAIN,AL_GAIN/10);
	alListenerf(AL_GAIN, 1/t); 

}
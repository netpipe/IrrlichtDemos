// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "sound_openal.h"

#ifdef HC1_ENABLE_OPENAL
#include "main.h"
#include "config.h"
#include "logging.h"
#include "globals.h"
#include <iostream>

using namespace std;
using namespace irr;


//void DisplayAlutError(char* string_, ALenum error_)
//{
//  ALenum error = alutGetError();
//    printf("%s", string_);
//    printf("%s\n", alutGetErrorString(error_));
//}


// --------------------------------------------------------
Sample::Sample()
{
    mSoundRef = 0;
    mBufferHandle = 0;
    mBufferIsCached = false;
    mSourceHandle = 0;
    mFadeOutPerMs = 0.f;
}

void Sample::FreeHandles()
{
    alSourceStop(mSourceHandle);
    SoundOpenAL::CheckOpenAlError("Sample::FreeHandles alSourceStop");

    alSourcei(mSourceHandle, AL_BUFFER, 0);  // detach buffer
    SoundOpenAL::CheckOpenAlError("Sample::FreeHandles alSourcei");

    alDeleteSources(1, &mSourceHandle);
    SoundOpenAL::CheckOpenAlError("Sample::FreeHandles alDeleteSources");

    if ( !mBufferIsCached )
    {
        alDeleteBuffers(1, &mBufferHandle);
        SoundOpenAL::CheckOpenAlError("Sample::FreeHandles alDeleteBuffers");
    }
}

// --------------------------------------------------------
SoundOpenAL::SoundOpenAL()
: mAlutInitialized(false)
, mTimeMs(0)
{
	HC1_PROFILE(CProfileScope ps1(L"SoundOpenAL", L"STARTUP");)
    PROFILE_ADD_GROUP(PGROUP_SOUND, "sound");
    PROFILE_ADD(500, PGROUP_SOUND, "playsample1");
    PROFILE_ADD(501, PGROUP_SOUND, "playsample3d");
    PROFILE_ADD(502, PGROUP_SOUND, "playmusic1");
    PROFILE_ADD(503, PGROUP_SOUND, "loadwav");
    PROFILE_ADD(504, PGROUP_SOUND, "playsample2");
    PROFILE_ADD(505, PGROUP_SOUND, "playsample3");
    PROFILE_ADD(506, PGROUP_SOUND, "playmusic2");
    PROFILE_ADD(507, PGROUP_SOUND, "oggstream");

    mHighestSoundRef = 0;

	// Init openAL
	if ( !alutInit(0, 0) )
	{
	    LOG.Error("alutInit failed\n");
	    mAlutInitialized = false;
	}
	else
	{
	    mAlutInitialized = true;
	}
    CheckOpenAlError("alutInit");

    // some more debug info
    LOG.LogLn(LP_DEBUG, "OpenAL default device:", alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER) );
    ALCcontext * context = alcGetCurrentContext();
    if ( context )
    {
        LOG.LogLn(LP_DEBUG, "Got a sound context");
        ALCdevice * device = alcGetContextsDevice(context);
        if ( device )
        {
            LOG.LogLn(LP_DEBUG, "Got sound device:", alcGetString(NULL, ALC_DEVICE_SPECIFIER) );
        }
        else
        {
            LOG.LogLn(LP_WARN, "Failed to get a sound device");
        }
    }
    else
    {
        LOG.LogLn(LP_WARN, "Failed to get a sound context");
    }
}

SoundOpenAL::~SoundOpenAL()
{
#ifdef HC1_ENABLE_OGG
    mOggStream.Release();
#endif
    for ( size_t i=0; i<mSamples.size(); ++i )
    {
        mSamples[i].FreeHandles();
    }
    ClearCache();

	// Exit OpenAL
	alutExit();
}

bool SoundOpenAL::IsAvailable()
{
    return mAlutInitialized;
}

void SoundOpenAL::Init()
{
    if ( !mAlutInitialized )
        return;

//    // testcode - hello world
//    ALuint helloBuffer, helloSource;
//    helloBuffer = alutCreateBufferHelloWorld ();
//    alGenSources (1, &helloSource);
//    alSourcei (helloSource, AL_BUFFER, helloBuffer);
//    alSourcePlay (helloSource);

    mSettings.ReadFromXml(APP.GetConfig()->GetSoundSettings());

    ALfloat		listenerPos[]={0.0,0.0,0.0};				// At the origin
	ALfloat		listenerVel[]={0.0,0.0,0.0};				// The velocity (no doppler here)
	ALfloat		listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};	// LookAt then Up

	// Set Listener attributes
	// Position ...
	alListenerfv(AL_POSITION,listenerPos);
	if ( !CheckOpenAlError("alListenerfv AL_POSITION") )
	{
        return;
	}

	// Velocity ...
	alListenerfv(AL_VELOCITY,listenerVel);
	if ( !CheckOpenAlError("alListenerfv AL_VELOCITY") )
	{
		return;
	}

	// Orientation ...
	alListenerfv(AL_ORIENTATION,listenerOri);
	if ( !CheckOpenAlError("alListenerfv AL_ORIENTATION") )
		return;

    alDistanceModel( mSettings.mDistanceModel );

    LogInfo();
}

void SoundOpenAL::LogInfo()
{
    LOG.Info("OPEN_AL\n");

    LOG.InfoLn("AL_VERSION: ", alGetString(AL_VERSION) );
    LOG.InfoLn("AL_RENDERER: ", alGetString(AL_RENDERER) );
    LOG.InfoLn("AL_VENDOR: ", alGetString(AL_VENDOR) );
//	LOG.InfoLn("AL_EXTENSIONS", alGetString(AL_EXTENSIONS) );   // does access uninitialized memory internally!
	CheckOpenAlError("PrintInfo");
}

void SoundOpenAL::Update(const irr::core::vector3df &pos_, const irr::core::vector3df &at_, const irr::core::vector3df &up_, unsigned int timeMs_)
{
    if ( !mAlutInitialized )
        return;

    int timeDiff = 0;
    if ( mTimeMs != 0 )
        timeDiff = timeMs_ - mTimeMs;
    mTimeMs = timeMs_;

    ALfloat	listenerOri[6];
    listenerOri[0] = at_.X;
    listenerOri[1] = at_.Y;
    listenerOri[2] = -at_.Z;
    listenerOri[3] = up_.X;
    listenerOri[4] = up_.Y;
    listenerOri[5] = -up_.Z;
    alListener3f(AL_POSITION, pos_.X, pos_.Y, -pos_.Z);

    ALfloat	listenerVel[3];
    float VEL_FACTOR = 0.05f;	// experimental, could be put in config
    listenerVel[0] = (pos_.X - mOldPos.X)*(float)timeDiff*VEL_FACTOR;
    listenerVel[1] = (pos_.Y - mOldPos.Y)*(float)timeDiff*VEL_FACTOR;
    listenerVel[2] = -(pos_.Z - mOldPos.Z)*(float)timeDiff*VEL_FACTOR;
    alListenerfv(AL_VELOCITY,listenerVel);
    CheckOpenAlError("SoundOpenAL::Update: alListenerfv AL_VELOCITY");

    mOldPos = pos_;

    CheckOpenAlError("SoundOpenAL::Update: alListenerfv AL_POSITION");
    alListenerfv(AL_ORIENTATION,listenerOri);
	CheckOpenAlError("SoundOpenAL::Update: alListenerfv AL_ORIENTATION");

    std::vector<Sample>::iterator it;
    for ( it = mSamples.begin(); it != mSamples.end(); )
    {
        Sample & sample = *it;
        int	sourceState;
        alGetSourcei(sample.mSourceHandle, AL_SOURCE_STATE, &sourceState);
        CheckOpenAlError("SoundOpenAL::Update alGetSourcei");
        if ( sourceState != AL_PLAYING )
        {
            sample.FreeHandles();
            it = mSamples.erase(it);
            continue;
        }
        else if ( sample.mFadeOutPerMs > 0.f )
		{
			float gain = 0.f;
			alGetSourcef(sample.mSourceHandle, AL_GAIN, &gain);
			gain -= sample.mFadeOutPerMs * (float)timeDiff;
			if ( gain <= 0.f )
			{
				sample.FreeHandles();
				it = mSamples.erase(it);
				continue;
			}
			else
			{
				alSourcef(sample.mSourceHandle, AL_GAIN,  gain );
			}
		}
		++it;
    }

#ifdef HC1_ENABLE_OGG
PROFILE_START(507);
    mOggStream.Update(mSettings);
PROFILE_STOP(507);
#endif
}

bool SoundOpenAL::LoadSample(const char *fileName_, Sample &targetSample, bool checkCache_)
{
    if ( !mAlutInitialized )
        return false;

    if( checkCache_ )
    {
        SampleMapIt it = mSampleCache.find( std::string(fileName_) );
        if ( it != mSampleCache.end() )
        {
            targetSample.mBufferHandle = it->second;
            targetSample.mBufferIsCached = true;
            return true;
        }
    }
    targetSample.mBufferIsCached = false;

#if OLD_ALUT_VERSION
    alGenBuffers(1, &targetSample.mBufferHandle);
    if ( !LoadAndAssignWAV(fileName_, targetSample.mBufferHandle) )
    {
        alDeleteBuffers(1, &targetSample.mBufferHandle);
        return false;
    }
#else
	if ( mIrrFileSystem )
	{
		irr::io::IReadFile* file = mIrrFileSystem->createAndOpenFile(irr::io::path(fileName_));
		if (!file )
		{
			LOG.LogLn(LP_DEBUG, "SoundOpenAL::LoadSample - createAndOpenFile failed for: ", fileName_);
			return false;
		}

		ALsizei length = (ALsizei)file->getSize();
		if ( length == 0 )
		{
			file->drop();
			LOG.LogLn(LP_DEBUG, "SoundOpenAL::LoadSample - filesize is 0 for: ", fileName_);
			return false;
		}

		char* buf = new char[ length];
		file->read(buf, length);

		targetSample.mBufferHandle = alutCreateBufferFromFileImage (buf, length);

		delete[] buf;
		file->drop();
	}
	else
	{
#ifdef _IRR_ANDROID_PLATFORM_
		LOG.LogLn(LP_DEBUG, "SoundOpenAL::LoadSample - mIrrFileSystem is NULL");
#endif
		targetSample.mBufferHandle = alutCreateBufferFromFile(fileName_);
	}
    if ( targetSample.mBufferHandle == AL_NONE )
    {
        return false;
    }
#endif
    return true;
}


int SoundOpenAL::PlaySample(const char *name_, float gain_, bool loop_)
{
    if ( !mAlutInitialized )
        return -1;

    PROFILE_START(500);
    Sample sample;

    if ( !LoadSample(name_, sample) )
    {
        return -1;
    }
    PROFILE_STOP(500);

	// Create source and attach buffer to it
	PROFILE_START(504);
    alGenSources( 1, &sample.mSourceHandle );
    CheckOpenAlError("PlaySample alGenSources");
	alSourcei(sample.mSourceHandle, AL_BUFFER, sample.mBufferHandle);
    if ( !CheckOpenAlError("PlaySample AL_BUFFER") )
    {
        PROFILE_STOP(504);
        return -1;
    }
    PROFILE_STOP(504);

    PROFILE_START(505);
    sample.mSoundRef = ++mHighestSoundRef;

    if ( gain_ < 0.f )
        gain_ = 0.f;
    ALfloat sourcePos[] = {0.0f, 0.0f, 0.0f};
    alSourcei (sample.mSourceHandle, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcef (sample.mSourceHandle, AL_ROLLOFF_FACTOR,  0.0 ); // position independent
    alSourcefv(sample.mSourceHandle, AL_POSITION, sourcePos);
    alSourcei (sample.mSourceHandle, AL_LOOPING,  loop_);
    alSourcePlay(sample.mSourceHandle);
    alSourcef (sample.mSourceHandle, AL_GAIN,  gain_*mSettings.mSfxVolume );
    CheckOpenAlError("SoundOpenAL::PlaySample");

    mSamples.push_back(sample);

    PROFILE_STOP(505);
    return sample.mSoundRef;
}

int SoundOpenAL::PlaySample3D(const char *name_, const irr::core::vector3df &pos_, float gain_, bool loop_)
{
    if ( !mAlutInitialized )
        return -1;

    PROFILE_START(501);
    Sample sample;

    if ( !LoadSample(name_, sample) )
    {
        PROFILE_STOP(501);
        return -1;
    }

	// Create source and attach buffer to it
    alGenSources( 1, &sample.mSourceHandle );
	alSourcei(sample.mSourceHandle, AL_BUFFER, sample.mBufferHandle);
    if ( !CheckOpenAlError("PlaySample genSources+attach buffer") )
    {
        PROFILE_STOP(501);
        return -1;
    }

    sample.mSoundRef = ++mHighestSoundRef;

    if ( gain_ < 0.f )
        gain_ = 0.f;
    ALfloat sourcePos[3];

    sourcePos[0] = pos_.X;
    sourcePos[1] = pos_.Y;
    sourcePos[2] = -pos_.Z;


    alSourcef (sample.mSourceHandle, AL_MAX_DISTANCE,  mSettings.mMaxDist );
    alSourcef (sample.mSourceHandle, AL_REFERENCE_DISTANCE, mSettings.mReferenceDist );
    alSourcef (sample.mSourceHandle, AL_ROLLOFF_FACTOR,  mSettings.mRolloffFactor );
    alSourcefv(sample.mSourceHandle, AL_POSITION, sourcePos);
    alSourcei (sample.mSourceHandle, AL_LOOPING,  loop_);
    alSourcePlay(sample.mSourceHandle);
    alSourcef (sample.mSourceHandle, AL_GAIN,  gain_*mSettings.mSfxVolume );
    CheckOpenAlError("SoundOpenAL::PlaySample");

    mSamples.push_back( sample );

    PROFILE_STOP(501);
    return sample.mSoundRef;
}

void SoundOpenAL::StopSample(int soundRef_)
{
    std::vector<Sample>::iterator it;
    for ( it = mSamples.begin(); it != mSamples.end(); ++it )
    {
        if ( (*it).mSoundRef == soundRef_ )
        {
            (*it).FreeHandles();
            mSamples.erase(it);
            return;
        }
    }
}

void SoundOpenAL::StopAllSamples()
{
    std::vector<Sample>::iterator it;
    for ( it = mSamples.begin(); it != mSamples.end(); ++it )
    {
		(*it).FreeHandles();
    }
    mSamples.clear();
}

void SoundOpenAL::FadeoutSample(int soundRef_, int timeMs_)
{
   Sample * sample = FindSample(soundRef_);
    if ( !sample )
        return;

    float gain;
    alGetSourcef(sample->mSourceHandle, AL_GAIN, &gain);
    if ( !timeMs_ )
        sample->mFadeOutPerMs = gain;
    else
        sample->mFadeOutPerMs = gain / (float)timeMs_;
}

bool SoundOpenAL::IsPlaying(int soundRef_)
{
    if ( !mAlutInitialized )
        return false;

    Sample * sample = FindSample(soundRef_);
    if ( !sample )
        return false;

    int	sourceState;
    alGetSourcei(sample->mSourceHandle, AL_SOURCE_STATE, &sourceState);
    if ( sourceState != AL_PLAYING )
        return false;

    return true;
}

void SoundOpenAL::SetGain(int soundRef_, float gain_, bool overrideFadeout_)
{
    Sample * sample = FindSample(soundRef_);
    if ( !sample )
        return;

    if ( !overrideFadeout_ && sample->mFadeOutPerMs > 0.f )
        return;

    if ( gain_ < 0.f )
        gain_ = 0.f;
    alSourcef (sample->mSourceHandle, AL_GAIN,  gain_*mSettings.mSfxVolume );
    CheckOpenAlError("SoundOpenAL::SetGain");
}

void SoundOpenAL::SetPosition(int soundRef_, const irr::core::vector3df &pos_)
{
    Sample * sample = FindSample(soundRef_);
    if ( !sample )
        return;

    ALfloat sourcePos[] = {pos_.X, pos_.Y, -pos_.Z};
    alSourcefv(sample->mSourceHandle, AL_POSITION, sourcePos);
    CheckOpenAlError("SoundOpenAL::SetPosition");
}

Sample* SoundOpenAL::FindSample(int soundRef_)
{
    std::vector<Sample>::iterator it;
    for ( it = mSamples.begin(); it != mSamples.end(); ++it )
    {
        if ( (*it).mSoundRef == soundRef_ )
        {
            return &(*it);
        }
    }

    return NULL;
}

#if OLD_ALUT_VERSION
// needed for mingw port
bool SoundOpenAL::LoadAndAssignWAV(const char* _fileName, ALuint _buffer)
{
    PROFILE_START(503);
	int			error;
	ALenum		format;
	ALsizei		size;
	ALsizei		freq;
	ALboolean	loop;
	ALvoid*		data;

	// Load in the WAV file from disk
	alutLoadWAVFile((ALbyte*)_fileName, &format, &data, &size, &freq, &loop);
	if ( !CheckOpenAlError("alutLoadWAVFile") )
	{
	    PROFILE_STOP(503);
        return false;
	}

	// Copy the new WAV data into the buffer
	alBufferData(_buffer,format,data,size,freq);
	if ( !CheckOpenAlError("alBufferData ") )
	{
	    PROFILE_STOP(503);
        return false;
	}

	// Unload the WAV file
	alutUnloadWAV(format,data,size,freq);
	if ( !CheckOpenAlError("alutUnloadWAV ") )
	{
	    PROFILE_STOP(503);
        return false;
    }

    PROFILE_STOP(503);
	return true;
}
#endif // #if OLD_ALUT_VERSION

bool SoundOpenAL::CheckOpenAlError(const char* string_)
{
    int error = alGetError();
    if (error != AL_NO_ERROR)
    {
        if ( string_ && strlen(string_) )
        {
            LOG.LogLn(LP_WARN, "OpenAL error in ", string_);
        }
        else
        {
            LOG.Log(LP_WARN, "OpenAL error: ");
        }
        switch (error)
        {
            case AL_INVALID_NAME:
                LOG.LogLn(LP_WARN, "AL_INVALID_NAME");
                break;
            case AL_INVALID_ENUM:
                LOG.LogLn(LP_WARN, "AL_INVALID_ENUM");
                break;
            case AL_INVALID_VALUE:
                LOG.LogLn(LP_WARN, "AL_INVALID_VALUE");
                break;
            case AL_INVALID_OPERATION:
                LOG.LogLn(LP_WARN, "AL_INVALID_OPERATION");
                break;
            case AL_OUT_OF_MEMORY:
                LOG.LogLn(LP_WARN, "AL_OUT_OF_MEMORY");
                break;
            default:
                LOG.LogLn(LP_WARN, "unknown error");
                break;
        };
        return false;
    }
    return true;
}

void SoundOpenAL::PlayMusic(const char *name_)
{
    if ( !mAlutInitialized )
        return;

#ifdef HC1_ENABLE_OGG
    PROFILE_START(502);
    LOG.LogLn(LP_DEBUG, "release old stream");
    mOggStream.Release();
    LOG.LogLn(LP_DEBUG, "open name: ", name_);
    if ( !mOggStream.Open(std::string(name_), mIrrFileSystem) )
    {
        PROFILE_STOP(502);
        return;
    }
    PROFILE_STOP(502);
    PROFILE_START(506);
    LOG.LogLn(LP_DEBUG, "start playback");
    mOggStream.Playback();
    PROFILE_STOP(506);
#endif
}

void SoundOpenAL::StopMusic()
{
#ifdef HC1_ENABLE_OGG
    mOggStream.Release();
#endif
}

bool SoundOpenAL::IsMusicPlaying()
{
    if ( !mAlutInitialized )
        return false;

#ifdef HC1_ENABLE_OGG
    return mOggStream.IsPlaying();
#else
	return false;
#endif
}

bool SoundOpenAL::CacheSample(const char *name_)
{
    RemoveSampleFromCache(name_);
    Sample sample;
    if ( LoadSample(name_, sample, false) )
    {
        mSampleCache[std::string(name_)] = sample.mBufferHandle;
        return true;
    }
    return false;
}

void SoundOpenAL::RemoveSampleFromCache(const char *name_)
{
    std::string strName(name_);
    SampleMapIt it = mSampleCache.find( strName );
    if ( it != mSampleCache.end() )
    {
        alDeleteBuffers(1, &(it->second));
        mSampleCache.erase(it);
    }
}

void SoundOpenAL::ClearCache()
{
    for ( SampleMapIt it = mSampleCache.begin(); it != mSampleCache.end(); ++it )
    {
        alDeleteBuffers(1, &(it->second));
    }
    mSampleCache.clear();
}

#endif // HC1_ENABLE_OPENAL


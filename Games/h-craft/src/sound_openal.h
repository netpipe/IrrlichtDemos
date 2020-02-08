// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef SOUND_OPENAL_H
#define SOUND_OPENAL_H

#include "compile_config.h"
#include "sound.h"

#ifdef HC1_ENABLE_OPENAL

#include "ogg_stream.h"

// OpenAL includes
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <vector>
#include <map>
#include <string>

struct Sample
{
    Sample();
    void FreeHandles();

    int mSoundRef;
    ALuint  mBufferHandle;
    bool    mBufferIsCached;
    ALuint  mSourceHandle;
    float   mFadeOutPerMs;
};

class SoundOpenAL : public ISound
{
public:
    SoundOpenAL();
    virtual ~SoundOpenAL();

    virtual void Init();
    virtual bool IsAvailable();

    virtual void Update(const irr::core::vector3df &pos_, const irr::core::vector3df &at_, const irr::core::vector3df &up_, unsigned int timeMs_);

    // currently only one stream file can be played
    virtual void PlayMusic(const char *name_);
    virtual void StopMusic();
    virtual bool IsMusicPlaying();

    virtual bool CacheSample(const char *name_);
    virtual void RemoveSampleFromCache(const char *name_);
    virtual void ClearCache();

    virtual int PlaySample(const char *name_, float gain_, bool loop_);
    virtual int PlaySample3D(const char *name_, const irr::core::vector3df &pos_, float gain_, bool loop_);

    virtual bool IsPlaying(int soundRef_);
    virtual void StopSample(int soundRef_);
    virtual void StopAllSamples();
    virtual void FadeoutSample(int soundRef_, int timeMs_);
    virtual void SetGain(int soundRef_, float gain_, bool overrideFadeout_=false);
    virtual void SetPosition(int soundRef_, const irr::core::vector3df &pos_);

    void LogInfo();
    static bool CheckOpenAlError(const char* string_=0);

protected:
    void DisplayOpenALError(char* string_, ALenum error);
    Sample* FindSample(int soundRef_);
#if OLD_ALUT_VERSION
    bool LoadAndAssignWAV(const char* fileName_, ALuint buffer_);
#endif
    bool LoadSample(const char *fileName_, Sample &targetSample, bool checkCache_=true);

private:
    bool mAlutInitialized;
    int mHighestSoundRef;
    unsigned int mTimeMs;
    irr::core::vector3df mOldPos;
    std::vector<Sample> mSamples;
    typedef std::map<std::string, ALuint> SampleMap;
    typedef std::map<std::string, ALuint>::iterator SampleMapIt;
    SampleMap mSampleCache;
#ifdef HC1_ENABLE_OGG
    OggStream   mOggStream;
#endif
};

#endif // HC1_ENABLE_OPENAL

#endif // SOUND_OPENAL_H

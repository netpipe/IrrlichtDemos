// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef SOUND_H
#define SOUND_H

#include "sound_settings.h"
#include <irrlicht.h>   // for irr::core::vector

// Interface for a sound implementation
// Also has empty dummy-functions so it can be used directly.
class ISound
{
public:
	ISound() : mIrrFileSystem(0) {}
    virtual ~ISound() {}

    virtual void Init() {}
    virtual bool IsAvailable() { return false; }

    virtual void Update(const irr::core::vector3df &pos_, const irr::core::vector3df &at_, const irr::core::vector3df &up_, unsigned int timeMs_) {}

    // currently only one stream file can be played
    virtual void PlayMusic(const char *name_) {}
    virtual void StopMusic() {}
    virtual bool IsMusicPlaying() { return false; }

    virtual bool CacheSample(const char *name_) { return false; }
    virtual void RemoveSampleFromCache(const char *name_) {}
    virtual void ClearCache() {}

    virtual int PlaySample(const char *name_, float gain_, bool loop_) { return -1; }
    virtual int PlaySample3D(const char *name_, const irr::core::vector3df &pos_, float gain_, bool loop_) { return -1; }

    virtual bool IsPlaying(int soundRef_) { return false; }
    virtual void StopSample(int soundRef_) {}
    virtual void StopAllSamples() {}
    virtual void FadeoutSample(int soundRef_, int timeMs_) {}
    virtual void SetGain(int soundRef_, float gain_, bool overrideFadeout_=false) {}
    virtual void SetPosition(int soundRef_, const irr::core::vector3df &pos_) {}

    SoundSettings& GetSettings() { return mSettings; }
    void SetIrrFs(irr::io::IFileSystem * fs) { mIrrFileSystem = fs; }

protected:
    SoundSettings mSettings;
    irr::io::IFileSystem * mIrrFileSystem;
};

#endif // SOUND_H

#ifndef SOUNDFACTORY_H
#define SOUNDFACTORY_H

#ifndef __APPLE__
    #include <AL/al.h>
    #include <AL/alc.h>
#else
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#endif

#include <string>
#include <map>

#include "Ogg/OggFile.h"
#include "SoundSource/SoundSource.h"

#ifdef WIN32
#include <windows.h>
#endif

void *startSoundFactoryThread ( void * );

//! Global sound factory that facilitates the loading and playing of audio
class SoundFactory
{
	public:
        SoundFactory(const std::string &workingDirectory);
        ~SoundFactory();

        void setActive(bool active);
        bool isActive();

        unsigned long loadSound(const std::string &soundName, const bool &intoMemory);
        void unloadSound(const unsigned long &soundID);

        void playSound(const unsigned long &soundID, const bool &looping, const float &volume, const float &attenuation);
        void stopSound(const unsigned long &soundID);
        void continueSound(const unsigned long &soundID);

        void setVolume(const unsigned long &soundID, const float &volume);
        void setAttenuation(const unsigned long &soundID, const float &attenuation);
        void setLooping(const unsigned long &soundID, const bool &looping);
        void setPosition(const unsigned long &soundID, const float &x, const float &y, const float &z);

        bool hasFinished(const unsigned long &soundID);
        bool hasStopped(const unsigned long &soundID);

        double getTime(const unsigned long &soundID);
        double getLength(const unsigned long &soundID);

        void run();

        void tick();

        void flush();

        static SoundFactory* getInstance(){ return SoundFactory::instance; }
    private:
        static SoundFactory *instance;

        bool active;

        std::map<unsigned long, SoundSource*> sources;
        std::map<unsigned long, std::string> sourceFiles;
        std::map<std::string, unsigned long> sounds;
        std::map<unsigned long, OggFile*> files;
        std::map<std::string, unsigned long> fileRefs;

        unsigned long refCount;
        unsigned long sourceRefCount;

        std::string workingDirectory;

        ALCcontext *soundContext;
        ALCdevice *soundDevice;

        // Functions below here
        bool loadOgg(const std::string &fileName, const bool &intoMemory);
        void unloadOgg(const std::string &fileName);
};

#endif

#ifndef AZSOUNDFACTORY_H
#define AZSOUNDFACTORY_H

#include <string>

class azSoundFactory
{
    public:
        virtual ~azSoundFactory() {}

        virtual void setActive(bool active) = 0;
        virtual bool isActive() = 0;

        virtual unsigned long loadSound(const std::string &soundName) = 0;
        virtual void unloadSound(const unsigned long &soundID) = 0;

        virtual void playSound(const unsigned long &soundID, const bool &looping, const float &volume, const float &attenuation) = 0;
        virtual void stopSound(const unsigned long &soundID) = 0;
        virtual void continueSound(const unsigned long &soundID) = 0;

        virtual void setVolume(const unsigned long &soundID, const float &volume) = 0;
        virtual void setAttenuation(const unsigned long &soundID, const float &attenuation) = 0;
        virtual void setLooping(const unsigned long &soundID, const bool &looping) = 0;
        virtual void setPosition(const unsigned long &soundID, const float &x, const float &y, const float &z) = 0;

        virtual bool hasFinished(const unsigned long &soundID) = 0;
        virtual bool hasStopped(const unsigned long &soundID) = 0;

        virtual void tick() = 0;
        virtual void flush() = 0;
};

#endif

#include "CAudioDevice.h"

namespace agEngine
{
    namespace audio
    {
        CAudioDevice::CAudioDevice()
        : active(true), sourceCount(1)
        {
            if (!(sDevice = alcOpenDevice(NULL)))
            {
                active = false;
            }

            sContext = alcCreateContext(sDevice, NULL);
            alcMakeContextCurrent(sContext);

            ALfloat orient[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
            alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
            alListenerfv(AL_ORIENTATION, orient);
            alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);

            // Clear the error stack
            alGetError();
        }

        CAudioDevice::~CAudioDevice()
        {
            clear();
        }

        CAudioSource* CAudioDevice::createAudioSource(CAudioStream* stream)
        {
            CAudioSource* nSrc = new CAudioSource(this, stream);
            nSrc->setSourceId(sourceCount);
            ++sourceCount;
            sources.push_back(nSrc);
            return nSrc;
        }

        void CAudioDevice::addAudioSource(CAudioSource* newSource)
        {
            newSource->setSourceId(sourceCount);
            ++sourceCount;
            sources.push_back(newSource);
            newSource->grab();
        }

        CAudioStream* CAudioDevice::createAudioStream(const c8* filename, bool loadToMemory)
        {
            // need to write a clause for different file types in the future
            CAudioData* audioData = new CAudioOgg(filename, loadToMemory);
            CAudioStream* audStream = new CAudioStream(audioData);
            return audStream;
        }

        void CAudioDevice::playAll()
        {
            for (u32 i = 0; i < sources.size(); ++i)
                sources[i]->updateBuffer(false);
        }

        void CAudioDevice::clear()
        {
            for (u32 i = 0; i < sources.size(); ++i)
                sources[i]->drop();

            sources.clear();
        }

        u32 CAudioDevice::getNextAvailableSourceId()
        {
            u32 temp = sourceCount;
            ++sourceCount;
            return temp;
        }

        void CAudioDevice::setListenerPosition(const core::vector3d<float>& newPosition)
        {
            alListener3f(AL_POSITION, newPosition.X, newPosition.Y, newPosition.Z);
        }

        void CAudioDevice::setListenerVelocity(const core::vector3d<float>& newVelocity)
        {
            alListener3f(AL_VELOCITY, newVelocity.X, newVelocity.Y, newVelocity.Z);
        }

        void CAudioDevice::setListenerOrientation(const core::vector3d<float>& orient1, const core::vector3d<float>& orient2)
        {
            ALfloat orient[] = {orient1.X, orient1.Y, orient1.Z, orient2.X, orient2.Y, orient2.Z};
            alListenerfv(AL_ORIENTATION, orient);
        }
    }
}

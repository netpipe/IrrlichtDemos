#ifndef _CAUDIOSOURCE_H_
#define _CAUDIOSOURCE_H_

#include <irrlicht.h>
#include <AL/al.h>

#include "CAudioDevice.h"
#include "CAudioStream.h"

using namespace irr;

namespace agEngine
{
    namespace audio
    {
        class CAudioDevice;
        class CAudioStream;

        class CAudioSource : public virtual IReferenceCounted
        {
        public:
            CAudioSource(CAudioDevice* adevice, CAudioStream* data = 0);
            virtual ~CAudioSource();

            virtual void attachStream(CAudioStream* stream);
            virtual void detatchStream();
            virtual void rewind();
            virtual bool updateBuffer(bool firstPlay=true);

            virtual void setSourceId(u32 value);

            virtual bool isPlaying() const;
            virtual bool play();
            virtual void stop();
            virtual void pause();

            virtual void setLoop(bool loop);
            virtual void setVolume(f32 volume);
            virtual void setAttenuation(f32 atten);
            virtual void setRelative(bool flag);

            virtual void setPosition(const core::vector3d<f32> &newPos);

            virtual void setStreamPosition(u32 newPosition);
            virtual u32 getStreamPosition() const;
            virtual u32 getBufferSize() const;
            virtual c8* getBuffer();
            virtual CAudioStream* getStream();
            virtual bool isLooping() const;
        private:
            bool loop;
            f32 volume;
            f32 attn;

            core::vector3d<f32> position;

            c8* buffer;
            u32 streamPosition;
            u32 bufferSize;

            ALuint alBuffer[2];

            u32 sourceId;

            CAudioStream* stream;
        };
    }
}

#endif

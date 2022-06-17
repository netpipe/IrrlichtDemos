
#ifdef AgAudio2
#ifndef _CAUDIODATA_H_
#define _CAUDIODATA_H_

#include <irrlicht.h>

#include "CAudioSource.h"

using namespace irr;

namespace agEngine
{
    namespace audio
    {
        class CAudioSource;

        class CAudioData : public virtual IReferenceCounted
        {
        public:
            virtual s32 open(const core::stringc&, bool loadToMemory = false) = 0;
            virtual void close() = 0;
            virtual u32 getSampleCount() = 0;
            virtual u32 getPosition() = 0;
            virtual void setPosition(u32 newPosition) = 0;
            virtual f64 getLength() = 0;
            virtual f64 getCurrentTime() = 0;
            virtual void rewind() = 0;
            virtual s32 read(c8* buffer, s32 size, s32 bigEndian, s32 word, s32 signedByte, s32* bitstream) = 0;
            virtual void bufferthis(ALuint* buffer, c8* data, u32 size) = 0;
            virtual bool readSamples(CAudioSource* source, ALuint bufferId) = 0;
            virtual bool hasFinished() const = 0;
        };
    }
}

#endif
#endif

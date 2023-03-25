#ifndef _CAUDIOOGG_H_
#define _CAUDIOOGG_H_

#ifdef AgAudio2
#include <irrlicht.h>

#include <AL/al.h>

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "CAudioData.h"

using namespace irr;

namespace agEngine
{
    namespace audio
    {
        struct SOggFile
        {
            c8* dataPtr;
            u32 dataSize;
            u32 dataRead;
        };

        class CAudioOgg : public CAudioData
        {
        public:
            CAudioOgg(const core::stringc& filename, bool loadToMemory = false);
            ~CAudioOgg();

            s32 open(const core::stringc& filename, bool loadToMemory = false);
            void close();
            u32 getSampleCount();
            u32 getPosition();
            void setPosition(u32 newPosition);
            f64 getLength();
            f64 getCurrentTime();
            void rewind();
            s32 read(c8* buffer, s32 size, s32 bigEndian, s32 word, s32 signedByte, s32* bitstream);
            void bufferthis(ALuint* buffer, c8* data, u32 size);
            bool readSamples(CAudioSource* source, ALuint bufferId);
            bool hasFinished() const;
        private:
            FILE* file;
            SOggFile oggData;
            OggVorbis_File stream;
            ov_callbacks vorbisCallbacks;
            vorbis_info* info;

            ALenum format;

            bool loadedInMemory;
            bool finished;
            u32 bufferSize;
        };
    }
}

#endif
#endif

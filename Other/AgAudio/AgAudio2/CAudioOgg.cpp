
#ifdef AgAudio2
#include "CAudioOgg.h"

namespace agEngine
{
    namespace audio
    {
        size_t VorbisRead(void *ptr, size_t byteSize, size_t sizeToRead, void *datasource)
        {
            size_t spaceToEOF;
            size_t actualSizeToRead;
            SOggFile* vorbisData;

            vorbisData = (SOggFile*)datasource;

            spaceToEOF = vorbisData->dataSize - vorbisData->dataRead;
            if ((sizeToRead*byteSize) < spaceToEOF)
                actualSizeToRead = (sizeToRead*byteSize);
            else
                actualSizeToRead = spaceToEOF;

            if (actualSizeToRead)
            {
                memcpy(ptr, (char*)vorbisData->dataPtr + vorbisData->dataRead, actualSizeToRead);

                vorbisData->dataRead += (actualSizeToRead);
            }

            return actualSizeToRead;
        }

        int VorbisSeek(void *datasource, ogg_int64_t offset, int whence)
        {
            size_t spaceToEOF;
            ogg_int64_t	actualOffset;
            SOggFile* vorbisData;

            vorbisData = (SOggFile*)datasource;

            switch (whence)
            {
            case SEEK_SET:
                if (vorbisData->dataSize >= offset)
                    actualOffset = offset;
                else
                    actualOffset = vorbisData->dataSize;
                vorbisData->dataRead = (int)actualOffset;
                break;
            case SEEK_CUR:
                spaceToEOF = vorbisData->dataSize - vorbisData->dataRead;
                if (offset < spaceToEOF)
                    actualOffset = (offset);
                else
                    actualOffset = spaceToEOF;
                vorbisData->dataRead += actualOffset;
                break;
            case SEEK_END:
                vorbisData->dataRead = vorbisData->dataSize+1;
                break;
            default:
                printf("*** ERROR *** Unknown seek command in VorbisSeek\n");
                break;
            };

            return 0;
        }

        int VorbisClose(void *datasource)
        {

            SOggFile* vorbisData;
            vorbisData = (SOggFile*)datasource;
            delete [] vorbisData->dataPtr;
            return 1;
        }

        long VorbisTell(void *datasource)
        {
            SOggFile*	vorbisData;

            vorbisData = (SOggFile*)datasource;

            return vorbisData->dataRead;
        }

        CAudioOgg::CAudioOgg(const core::stringc& filename, bool loadToMemory)
        : finished(false)
        {
            open(filename, loadToMemory);
        }

        CAudioOgg::~CAudioOgg()
        {
            close();
        }

        s32 CAudioOgg::open(const core::stringc& filename, bool loadToMemory)
        {
            loadedInMemory = loadToMemory;

            if (!(file = fopen(filename.c_str(), "rb")))
            {
                loadedInMemory = true;
                return -1;
            }

            if (loadToMemory)
            {
                u32 fileSize = 0;
                c8 tempChar;
                while (!feof(file))
                {
                    tempChar = getc(file);
                    ++fileSize;
                }

                fseek(file, 0, SEEK_SET);
                oggData.dataPtr = new c8[fileSize];
                for (u32 i = 0; i < fileSize; ++i)
                    oggData.dataPtr[i] = getc(file);

                oggData.dataRead = 0;
                oggData.dataSize = fileSize;

                fclose(file);

                vorbisCallbacks.read_func = VorbisRead;
                vorbisCallbacks.close_func = VorbisClose;
                vorbisCallbacks.seek_func = VorbisSeek;
                vorbisCallbacks.tell_func = VorbisTell;

                ov_open_callbacks(&oggData, &stream, NULL, 0, vorbisCallbacks);
            }
            else
            {
                if (ov_open(file, &stream, NULL, 0) < 0)
                {
                    return -1;
                }
            }

            info = ov_info(&stream, -1);

            if (info->channels == 1)
            {
                format = AL_FORMAT_MONO16;
            }
            else
            {
                format = AL_FORMAT_STEREO16;
            }

            return 0;
        }

        void CAudioOgg::bufferthis(ALuint *buffer, c8* data, u32 size)
        {
            alBufferData(*buffer, format, data, size, info->rate);
        }

        u32 CAudioOgg::getSampleCount()
        {
            return ov_pcm_total(&stream, -1);
        }

        u32 CAudioOgg::getPosition()
        {
            return ov_pcm_tell(&stream);
        }

        void CAudioOgg::setPosition(u32 pos)
        {
            ov_pcm_seek(&stream, pos);
        }

        f64 CAudioOgg::getLength()
        {
            return ov_time_total(&stream, -1);
        }

        f64 CAudioOgg::getCurrentTime()
        {
            return ov_time_tell(&stream);
        }

        void CAudioOgg::rewind()
        {
            ov_pcm_seek(&stream, 0);
        }

        s32 CAudioOgg::read(c8* buffer, s32 size, s32 bigEndian, s32 word, s32 signedByte, s32* bitstream)
        {
            return ov_read(&stream, buffer, size, bigEndian, word, signedByte, bitstream);
        }

        void CAudioOgg::close()
        {
            if (!loadedInMemory)
                fclose(file);
        }

        bool CAudioOgg::readSamples(CAudioSource* source, ALuint bufferId)
        {
            s32 section, result;
            u32 size = 0;

            finished = false;

            u32 sBufferSize = source->getBufferSize();
            c8* sBuffer = source->getBuffer();

            setPosition(source->getStreamPosition());
            while (size < (sBufferSize * 16))
            {
                if (getPosition() >= getSampleCount())
                {
                    rewind();
                    if (!source->isLooping())
                        finished = true;

                    break;
                }

                result = read(sBuffer + size, (sBufferSize * 16) - size, 0, 2, 1, &section);

                if (result >= 0)
                    size += result;
            }
            source->setStreamPosition(getPosition());

            if (size == 0)
                return false;

            bufferthis(&bufferId, source->getBuffer(), size);

            if (alGetError() != AL_NO_ERROR)
            {
                return false;
            }

            return true;
        }

        bool CAudioOgg::hasFinished() const
        {
            return finished;
        }
    }
}
#endif

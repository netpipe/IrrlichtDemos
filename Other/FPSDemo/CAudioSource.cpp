#include "CAudioSource.h"

namespace agEngine
{
    namespace audio
    {
        CAudioSource::CAudioSource(CAudioDevice* adevice, CAudioStream* data)
        : loop(false), sourceId(0), stream(data)
        {
            if (data)
                data->grab();

            bufferSize = 32767;
            buffer = new c8[bufferSize*16];

            if (adevice)
                sourceId = adevice->getNextAvailableSourceId();

            alGenBuffers(2, alBuffer);

            // set defaults
            alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
            alSource3f(sourceId, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
            alSource3f(sourceId, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
            alSourcef(sourceId, AL_PITCH, 1.0f);
            alSourcef(sourceId, AL_GAIN, 1.0f);
            alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.0f);

            // Clear the error stack
            alGetError();
        }

        CAudioSource::~CAudioSource()
        {
            detatchStream();
        }

        void CAudioSource::attachStream(CAudioStream* stream)
        {
            detatchStream();

            this->stream = stream;
            stream->grab();
        }

        void CAudioSource::detatchStream()
        {
            if (stream)
                stream->drop();

            stream = 0;
        }

        void CAudioSource::setSourceId(u32 value)
        {
            sourceId = value;
            alGenSources(1, &sourceId);
        }

        void CAudioSource::rewind()
        {
            stream->rewind();
        }

        bool CAudioSource::updateBuffer(bool firstPlay)
        {
            if (!stream)
                return false;

            if (firstPlay)
            {
                if (!stream->readSamples(this, alBuffer[0]))
                    return false;

                alSourceQueueBuffers(sourceId, 1, &alBuffer[0]);

                if (!stream->readSamples(this, alBuffer[1]))
                {
                    if (stream->hasFinished())
                        return true;

                    return false;
                }

                alSourceQueueBuffers(sourceId, 1, &alBuffer[1]);

                return true;
            }

            ALenum state;
            alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
            if (state == AL_STOPPED || state == AL_PAUSED)
                return false;

            s32 done = 0;
            alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &done);

            while (done--)
            {
                ALuint buffer;
                alSourceUnqueueBuffers(sourceId, 1, &buffer);
                if (stream->hasFinished())
                {
                    stop();
                    continue;
                }
                if (!stream->readSamples(this, buffer))
                    return false;

                alSourceQueueBuffers(sourceId, 1, &buffer);
            }

            return true;
        }

        bool CAudioSource::isPlaying() const
        {
            ALenum state;
            alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
            return (state == AL_PLAYING);
        }

        bool CAudioSource::play()
        {
            if (!stream)
                return false;

            if (isPlaying())
                return true;

            streamPosition = 0;

            if (!updateBuffer(true))
                return false;

            alSourcePlay(sourceId);
            return true;
        }

        void CAudioSource::stop()
        {
            alSourceStop(sourceId);

            s32 done;
            alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &done);

            while (done--)
            {
                ALuint buffer;
                alSourceUnqueueBuffers(sourceId, 1, &buffer);
            }

            rewind();
        }

        void CAudioSource::pause()
        {
            alSourcePause(sourceId);
        }

        void CAudioSource::setLoop(bool flag)
        {
            loop = flag;
        }

        void CAudioSource::setVolume(f32 value)
        {
            if (value > 1.0f)
                volume = 1.0f;
            else if (value < 0.0f)
                volume = 0.0f;
            else
                volume = value;

            alSourcef(sourceId, AL_GAIN, volume);
        }

        void CAudioSource::setAttenuation(f32 value)
        {
            if (value < 0.0f)
                attn = 0.0f;
            else
                attn = value;

            alSourcef(sourceId, AL_ROLLOFF_FACTOR, attn);
        }

        void CAudioSource::setRelative(bool flag)
        {
            alSourcei(sourceId, AL_SOURCE_RELATIVE, flag?AL_TRUE:AL_FALSE);
        }

        void CAudioSource::setPosition(const core::vector3d<f32>& newPos)
        {
            alSource3f(sourceId, AL_POSITION, newPos.X, newPos.Y, newPos.Z);
            position = newPos;
        }

        u32 CAudioSource::getStreamPosition() const
        {
            return streamPosition;
        }

        void CAudioSource::setStreamPosition(u32 newPosition)
        {
            streamPosition = newPosition;
        }

        u32 CAudioSource::getBufferSize() const
        {
            return bufferSize;
        }

        c8* CAudioSource::getBuffer()
        {
            return buffer;
        }

        CAudioStream* CAudioSource::getStream()
        {
            return stream;
        }

        bool CAudioSource::isLooping() const
        {
            return loop;
        }
    }
}

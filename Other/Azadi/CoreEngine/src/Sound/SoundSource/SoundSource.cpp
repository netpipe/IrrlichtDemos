#include "SoundSource.h"

SoundSource::SoundSource()
{
    has_stopped=true;
}

bool SoundSource::hasFinished()
{
    return has_stopped;
}

double SoundSource::getLength()
{
    return (ogg->getLength());
}

double SoundSource::getCurrentTime()
{
    return (ogg->curTime());
}

bool SoundSource::bufferit(ALuint buffer)
{
    unsigned int segments = 32768;
    char data[524288];
    int size = 0;
    int section, result;

    ogg->setPosition(oggPos);

    while (unsigned(size) < (segments * 16))
    {
        #ifdef __powerpc__
        if (ogg->getPosition() >= ogg->samples() - 20)
        {
            if (looping)
                ogg->rewind();
            else if (size == 0)
            {
                ogg->rewind();
                stop();
            }
            else
                break;
        }
        result = ogg->read(data + size, (segments * 16) -size, 1, 2, 1, &section);
        #else
        if (ogg->getPosition() >= ogg->samples() - 20)
        {
            if(looping)
                ogg->rewind();
            else if (size == 0)
            {
                ogg->rewind();
                stop();
            }
            else
                break;
        }
        result = ogg->read(data + size, (segments * 16) -size, 0, 2, 1, &section);
        #endif

        if (result>0)
            size += result;
        else
            break;
    }

    if (size == 0)
        return false;

    ogg->bufferthis(&buffer, data, size);

    oggPos = ogg->getPosition();

    return true;
}

void SoundSource::update()
{
    int done;
    bool active = true;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &done);

    while (done--)
    {
        ALuint buffer;

        alSourceUnqueueBuffers(source, 1, &buffer);

        active = bufferit(buffer);

        alSourceQueueBuffers(source, 1, &buffer);
    }
}

bool SoundSource::isPlaying()
{
    ALenum state;

    alGetSourcei(source, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}

void SoundSource::play()
{
    if (isPlaying())
        return;

    if (!bufferit(buffers[0]))
        return;

    if (!bufferit(buffers[1]))
        return;

    alSourceQueueBuffers(source, 2, buffers);
    alSourcePlay(source);
    has_stopped = false;
}

void SoundSource::stop()
{
    alSourceStop(source);
    has_stopped = true;
}

void SoundSource::rewind()
{
    ogg->rewind();
    oggPos = ogg->getPosition();
}

void SoundSource::attachToSource(OggFile *loadedOgg, unsigned int currentSources)
{
    source = currentSources-1;
    ogg = loadedOgg;
    alGenBuffers(2, buffers);
    alGenSources(1, &source);

    alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
    alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
    alSource3f(source, AL_DIRECTION, 0.0, 0.0, 0.0);
    alSourcef(source, AL_PITCH, 1.0);
    alSourcef(source, AL_GAIN, 1.0);
    alSourcef(source, AL_ROLLOFF_FACTOR, 1.0);
}

void SoundSource::setAttenuation(float arg)
{
    if (arg < 0.0)
        arg = 0.0;
    alSourcef(source, AL_ROLLOFF_FACTOR, arg);
}

void SoundSource::setVolume(float arg)
{
    if (arg > 1.0)
        arg = 1.0;
    else if (arg < 0.0)
        arg = 0.0;

    volume = arg;
    alSourcef(source, AL_GAIN, volume);
}

void SoundSource::setLooping(bool loop)
{
    looping = loop;
    if (looping)
        alSourcef(source, AL_LOOPING, AL_TRUE);
    else
        alSourcef(source, AL_LOOPING, AL_FALSE);
}

void SoundSource::setPosition(const float &x, const float &y, const float &z)
{
    alSource3f(source, AL_POSITION, x, y, z);
}

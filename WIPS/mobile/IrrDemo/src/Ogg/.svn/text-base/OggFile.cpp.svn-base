#include "OggFile.h"

int OggFile::open(const std::string& filename)
{
    int temp;

    if (!(file = fopen(filename.c_str(), "rb")))
    {
        return -1;
    }

    if ((temp = ov_open(file, &stream, NULL, 0))<0)
    {
        return -1;
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

void OggFile::bufferthis(ALuint *buffer, char* data, int size)
{
    alBufferData(*buffer, format, data, size, info->rate);
}

long OggFile::samples()
{
    return ov_pcm_total(&stream, -1);
}

long OggFile::getPosition()
{
    return ov_pcm_tell(&stream);
}

void OggFile::setPosition(long pos)
{
    ov_pcm_seek(&stream, pos);
}

double OggFile::getLength()
{
    return ov_time_total(&stream, -1);
}

double OggFile::curTime()
{
    return ov_time_tell(&stream);
}

void OggFile::rewind()
{
    ov_pcm_seek(&stream, 0);
}

int OggFile::read(char* data, int size, int i , int h , int j, int *section)
{
    return ov_read(&stream, data, size, i, h, h, section);
}

void OggFile::close()
{
    fclose(file);
}

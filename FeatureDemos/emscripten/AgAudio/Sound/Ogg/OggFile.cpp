#include "OggFile.h"
#include <memory.h>
// Make sure to fix this header link
//#include "../../Encryption/encUserTools.h"

// Disable encryption for now!
//#define ENCRYPTED 1

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

int OggFile::open(const std::string& filename, const bool &loadToMemory)
{
	loadedInMemory = loadToMemory;

    if (!(file = fopen(filename.c_str(), "rb")))
    {
		loadedInMemory = true;
        return -1;
    }

    if (loadToMemory)
    {
        unsigned int fileSize = 0;
        char tempChar;
        while (!feof(file))
        {
            tempChar = getc(file);
            fileSize++;
        }

//		std::rewind(file);
#ifdef ENCRYPTED
		--fileSize;
		char *tempData = new char[fileSize];
		char padding = getc(file);
		for (unsigned int i = 0; i < fileSize; ++i)
        {
            tempData[i] = getc(file);
        }

		oggData.dataPtr = new char[fileSize - padding];
		Encryption::UserTools::bCipher::instance->decipher(tempData, fileSize, oggData.dataPtr, padding);
		delete [] tempData;

		oggData.dataRead = 0;
        oggData.dataSize = fileSize-padding;
#else
        oggData.dataPtr = new char[fileSize];
        for (unsigned int i = 0; i < fileSize; ++i)
        {
            oggData.dataPtr[i] = getc(file);
        }

        oggData.dataRead = 0;
        oggData.dataSize = fileSize;
#endif

        fclose(file);

        vorbisCallbacks.read_func = VorbisRead;
        vorbisCallbacks.close_func = VorbisClose;
        vorbisCallbacks.seek_func = VorbisSeek;
        vorbisCallbacks.tell_func = VorbisTell;

        ov_open_callbacks(&oggData, &stream, NULL, 0, vorbisCallbacks);
    }
    else
    {
        if (ov_open(file, &stream, NULL, 0)<0)
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
	if (!loadedInMemory)
		fclose(file);
}

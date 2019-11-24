#ifndef OGG_H
#define OGG_H

#ifdef __APPLE__
    #include <OpenAL/al.h>
#else
    #include <AL/al.h>
#endif

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <iostream>

class OggFile
{
	public:
		int open(const std::string&);
		void close();
		long samples();
		long getPosition();
		void setPosition(long);
		double getLength();
		double curTime();
		void rewind();
		int read(char*, int, int, int,int, int*);
		void bufferthis(ALuint*, char*, int);
	private:

		FILE* file;
		OggVorbis_File stream;
		vorbis_info* info;

		ALuint buffers[2];
		ALenum format;
};

#endif

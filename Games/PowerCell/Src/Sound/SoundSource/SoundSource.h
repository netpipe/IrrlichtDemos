#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#ifndef __APPLE__
    #include <AL/al.h>
#else
    #include <OpenAL/al.h>
#endif

#include "../Ogg/OggFile.h"

class SoundSource
{
	public:
        SoundSource();
		void update();
		bool isPlaying();
		bool hasFinished();
		double getLength();
		double getCurrentTime();
		void play();
		void stop();
		void rewind();
		void attachToSource(OggFile*, unsigned int);
		void setAttenuation(float);
		void setVolume(float);
		void setLooping(bool);
		void setPosition(const float &x, const float &y, const float &z);
	private:
		float volume;
		bool looping;
		bool has_stopped;
		bool bufferit(ALuint);
		unsigned int source;
		OggFile *ogg;
		ALuint buffers[2];
		long oggPos;
};

#endif

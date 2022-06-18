#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <map>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

#include "Ogg/OggFile.h"

namespace agEngine
{
	class AudioSource;
	class SoundData;

	class AudioDevice
	{
	public:
		AudioDevice();

		AudioSource *createASource(SoundData *soundData);
		void attachAudioSource(AudioSource *aSource);

		void playAll();
		void flush();

		void setListenerPosition(const float &x, const float &y, const float &z);
		void setListenerVelocity(const float &x, const float &y, const float &z);
		void setListenerOrient(const float &x, const float &y, const float &z);

		AudioDevice *getInstance() { return instance; }

		void drop();

		static AudioDevice *instance;

		float lpX, lpY, lpZ;
		float lvX, lvY, lvZ;
		float loX, loY, loZ;
	protected:
		~AudioDevice();
	private:
        ALCcontext *soundContext;
        ALCdevice *soundDevice;

		bool active;

		// Pointers to the data
		unsigned int sourceCount;
		std::map<unsigned int, AudioSource *> audioSourcePointers;

		friend class AudioSource;
		friend class SoundData;
	};

	class AudioSource
	{
	public:
		AudioSource(AudioDevice *device=0, const unsigned int &sourceId=0, const unsigned int &bufferSize=32768);

		void attachSound(SoundData *soundData);
		bool play();
		void stop();
		void pause();
		void rewind();
		bool isPlaying() const;

		void setLoop(const bool &loop);
		void setVolume(const float &volume);
		void setAttenuation(const float &attn);
		void setRelative(const bool &relative);

		void setPosition(const float &x, const float &y, const float &z);

		void drop();

		float X,Y,Z;

	protected:
		~AudioSource();

	private:
		bool updateBuffer(const bool &firstPlay = false);
		void setSourceId(const unsigned int &sourceId);

		unsigned int sourceId;

		SoundData *sData;
		AudioDevice *device;

		ALuint alBuffer[2];

		unsigned int bufferSize;
		char *buffer;
		unsigned int soundPosition;

		bool loop;
		float volume;
		float attn;

		friend class AudioDevice;
		friend class SoundData;
	};

	class SoundData
	{
	public:
		static SoundData *LoadAudio(const std::string &fileName, const bool &loadToMemory);

		void attachToAudioSource(AudioSource *audioSource);

		void drop();

	protected:
		~SoundData();

	private:
		SoundData(const std::string &fileName, const bool &loadToMemory);

		void rewind();
		bool readSamples(ALuint bufferId);

		unsigned int soundId;
		unsigned int refCount;

		AudioDevice *device;
		AudioSource *source;

		OggFile *ogg;

		friend class AudioSource;
	};

	AudioDevice *createDevice(const bool &threaded);
}

#endif

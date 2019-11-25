#include "audio.h"

namespace agEngine
{
	AudioDevice* AudioDevice::instance = NULL;

	/*
		Main Audio Device
	*/
	AudioDevice::AudioDevice()
	{
		// Zero out the refcount
		sourceCount = 0;

		// Initialize the OpenAL backend
		if ( ! ( soundDevice = alcOpenDevice ( NULL ) ) )
		{
			active = false;
		}
		else
		{
			active = true;
			soundContext = alcCreateContext ( soundDevice, NULL );
			alcMakeContextCurrent ( soundContext );
		}

		ALfloat orient[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListenerfv(AL_ORIENTATION, orient);
		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	}

	// destructor that clears all the buffers
	AudioDevice::~AudioDevice()
	{
		audioSourcePointers.clear();
	}

	// Clears out all the loaded audio then destroys itself
	void AudioDevice::drop()
	{
		for (unsigned int i = 0; i < audioSourcePointers.size(); ++i)
			audioSourcePointers[i]->drop();

		delete this;
	}

	void AudioDevice::flush()
	{
		for (unsigned int i = 0; i < audioSourcePointers.size(); ++i)
			audioSourcePointers[i]->drop();

		audioSourcePointers.clear();
	}

	// Create a new source
	AudioSource* AudioDevice::createASource(SoundData *data)
	{
		AudioSource *source = new AudioSource(this, sourceCount);
		source->attachSound(data);
		audioSourcePointers[sourceCount++] = source;
		return source;
	}

	void AudioDevice::attachAudioSource(AudioSource *aSource)
	{
		aSource->device = this;
		aSource->setSourceId(sourceCount);
		audioSourcePointers[sourceCount++] = aSource;
	}

	void AudioDevice::playAll()
	{
		std::map<unsigned int, AudioSource *>::iterator it = audioSourcePointers.begin();
		while (it != audioSourcePointers.end())
			(*it++).second->updateBuffer();
	}

	void AudioDevice::setListenerPosition(const float &x, const float &y, const float &z)
	{
		alListener3f(AL_POSITION, x, y, z);
		lpX = x; lpY = y; lpZ = z;
	}

	void AudioDevice::setListenerVelocity(const float &x, const float &y, const float &z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
		lvX = x; lvY = y; lvZ = z;
	}

	void AudioDevice::setListenerOrient(const float &x, const float &y, const float &z)
	{
		alListener3f(AL_ORIENTATION, x, y, z);
		loX = x; loY = y; loZ = z;
	}

	/*
		Audio Sources
	*/
	AudioSource::AudioSource(AudioDevice *audioDevice = NULL, const unsigned int &newSourceId = 0, const unsigned int &buffSize)
	{
		device = audioDevice;
		sourceId = newSourceId;
		sData = NULL;
		bufferSize = buffSize;
		buffer = new char[bufferSize * 16];

		alGenBuffers(2, alBuffer);
		alGenSources(1, &sourceId);

		alSource3f(sourceId, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(sourceId, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSource3f(sourceId, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
		alSourcef(sourceId, AL_PITCH, 1.0f);
		alSourcef(sourceId, AL_GAIN, 1.0f);
		alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.0f);

		if (device != NULL)
			if (device->audioSourcePointers.find(sourceId) == device->audioSourcePointers.end())
				device->audioSourcePointers[sourceId] = this;
	}

	AudioSource::~AudioSource()
	{
		delete [] buffer;
	}

	// deletes one of the reference counts, and then deletes itself if it's the last reference
	void AudioSource::drop()
	{
		if (sData != NULL)
			sData->drop();

		if (device != NULL)
			device->audioSourcePointers.erase(device->audioSourcePointers.find(sourceId));

		delete this;
	}

	void AudioSource::attachSound(SoundData *soundData)
	{
		if (soundData == NULL)
			return;
		sData = soundData;
		sData->source = this;
		++sData->refCount;
	}

	void AudioSource::setSourceId(const unsigned int &newId)
	{
		sourceId = newId;
	}

	void AudioSource::rewind()
	{
		soundPosition = 0;
		if (sData != NULL)
			sData->rewind();
	}

	bool AudioSource::updateBuffer(const bool &firstPlay)
	{
		if (sData == NULL)
			return false;

		if (firstPlay)
		{
			if (!sData->readSamples(alBuffer[0]))
				return false;
			if (!sData->readSamples(alBuffer[1]))
				return false;

			alSourceQueueBuffers(sourceId, 2, alBuffer);
			return true;
		}

		ALenum state;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED || state == AL_PAUSED)
			return false;

		int done;
		alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &done);

		while (done--)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(sourceId, 1, &buffer);
			if (!sData->readSamples(buffer))
				return false;
			alSourceQueueBuffers(sourceId, 1, &buffer);
		}
		return true;
	}

	bool AudioSource::isPlaying() const
	{
		ALenum state;
		alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	bool AudioSource::play()
	{
		if (sData == NULL)
			return false;

		if (isPlaying())
			return true;

		// reset the sound position
		soundPosition = 0;

		if (!updateBuffer(true))
			return false;

		alSourcePlay(sourceId);
		return true;
	}

	void AudioSource::stop()
	{
		alSourceStop(sourceId);

		int done;
		alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &done);

		while (done--)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(sourceId, 1, &buffer);
		}

		rewind();
	}

	void AudioSource::pause()
	{
		alSourcePause(sourceId);
	}

	void AudioSource::setLoop(const bool &looping)
	{
		loop = looping;
	}

	void AudioSource::setVolume(const float &newVolume)
	{
		if (newVolume > 1.0f)
			volume = 1.0f;
		else if (newVolume < 0.0f)
			volume = 0.0f;
		else
			volume = newVolume;

		alSourcef(sourceId, AL_GAIN, volume);
	}

	void AudioSource::setAttenuation(const float &attenuation)
	{
		if (attenuation < 0.0f)
			attn = 0.0f;
		else
			attn = attenuation;

		alSourcef(sourceId, AL_ROLLOFF_FACTOR, attn);
	}

	void AudioSource::setPosition(const float &x, const float &y, const float &z)
	{
		alSource3f(sourceId, AL_POSITION, x, y, z);
		X = X; Y = y; Z = z;
	}

	void AudioSource::setRelative(const bool &relative)
	{
		alSourcei(sourceId, AL_SOURCE_RELATIVE, relative?AL_TRUE:AL_FALSE);
	}

	/* 
		Sound Data
	*/
	SoundData* SoundData::LoadAudio(const std::string &filename, const bool &loadToMemory = false)
	{
		FILE *f;
		if (!(f = fopen(filename.c_str(), "rb")))
		{
			printf("Could not load: %s\n", filename.c_str());
			return NULL;
		}
		else
		{
			fclose(f);
			return new SoundData(filename, loadToMemory);
		}
	}

	SoundData::SoundData(const std::string &filename, const bool &loadToMemory = false)
	{
		refCount = 1;
		std::string extension = filename.substr ( filename.size() - 3, 3 );
		if ( extension == "ogg" ) 
		{
			ogg = new OggFile();
			ogg->open(filename, loadToMemory);
		}
	}

	SoundData::~SoundData()
	{
		if (ogg != NULL)
		{
			ogg->close();
			delete ogg;
			ogg = NULL;
		}
	}

	void SoundData::drop()
	{
		if (--refCount == 0)
			delete this;
	}

	void SoundData::attachToAudioSource(AudioSource *audioSource)
	{
		audioSource->attachSound(this);
	}

	void SoundData::rewind()
	{
		if (ogg != NULL)
			ogg->rewind();
	}

	bool SoundData::readSamples(ALuint bufferId)
	{
		int section, result;
		unsigned int size = 0;

		if (ogg != NULL)
		{
			ogg->setPosition(source->soundPosition);
			while(size < (source->bufferSize * 16))
			{
				if (ogg->getPosition() >= ogg->samples() - 20)
				{
					if (source->loop)
						ogg->rewind();
					else if (size == 0)
					{
						ogg->rewind();
						source->stop();
					}
					else
						break;
				}
#ifdef __powerpc__
				result = ogg->read(source->buffer + size, (source->bufferSize * 16)-size, 1, 2, 1, &section);
#else
				result = ogg->read(source->buffer + size, (source->bufferSize * 16)-size, 0, 2, 1, &section);
#endif
				if (result >= 0)
					size+=result;
			}
			source->soundPosition = ogg->getPosition();
		}

		if (size == 0)
			return false;

		if (ogg != NULL)
			ogg->bufferthis(&bufferId, source->buffer, size);

		return true;
	}


	AudioDevice *createDevice(const bool &threaded)
	{
		if (AudioDevice::instance == NULL)
		{
			return AudioDevice::instance = new AudioDevice();
		}
		else
			return AudioDevice::instance;
	}
}

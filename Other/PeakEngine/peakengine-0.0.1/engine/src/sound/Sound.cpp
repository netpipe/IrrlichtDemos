/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "sound/Sound.h"
#include "sound/DataSoundSource.h"
#include "sound/OggVorbisSource.h"
#include "sound/SoundEngine.h"
#include "graphic/SceneNode.h"

#include <AL/al.h>

namespace peak
{
	Sound::Sound()
	{
		source = 0;
		oalsource = 0;
		cleanup = false;
		node = 0;
		adjustspeed = false;
	}
	Sound::~Sound()
	{
		destroy();
		SoundEngine::get()->removeSoundFromList(this);
	}
	
	bool Sound::init(SoundSource *source)
	{
		this->source = source;
		source->grab();
		
		if (!source->isStreamed())
		{
			// Simple source
			alGenSources(1, &oalsource);
			unsigned int buffer = ((DataSoundSource*)source)->getBuffer();
			
			alSourcei(oalsource, AL_BUFFER, buffer);
			alSourcef(oalsource, AL_PITCH, 1.0f);
			alSourcef(oalsource, AL_GAIN, 1.0f);
			alSourcef(oalsource, AL_MIN_GAIN, 0.1f);
			alSource3f(oalsource, AL_POSITION, 0, 0, 0);
			alSource3f(oalsource, AL_VELOCITY, 0, 0, 0);
			alSourcei(oalsource, AL_LOOPING, 0);
			
			alSourcePlay(oalsource);
		}
		else
		{
			// Buffered source
			alGenBuffers(3, buffers);
			alGenSources(1, &oalsource);
			
			alSourcef(oalsource, AL_PITCH, 1.0f);
			alSourcef(oalsource, AL_GAIN, 1.0f);
			alSource3f(oalsource, AL_POSITION, 0, 0, 0);
			alSource3f(oalsource, AL_VELOCITY, 0, 0, 0);
			
			((OggVorbisSource*)source)->fillBuffer(buffers[0]);
			((OggVorbisSource*)source)->fillBuffer(buffers[1]);
			((OggVorbisSource*)source)->fillBuffer(buffers[2]);
			alSourceQueueBuffers(oalsource, 3, buffers);
			
			alSourcePlay(oalsource);
		}
		
		return true;
	}
	bool Sound::destroy(void)
	{
		if (source)
		{
			if (source->isStreamed())
			{
				alDeleteBuffers(3, buffers);
			}
			alSourceStop(oalsource);
			alDeleteSources(1, &oalsource);
			source->drop();
			source = 0;
		}
		return true;
	}
	
	SoundSource *Sound::getSource(void)
	{
		return source;
	}
	
	void Sound::setLooped(bool looped)
	{
		if (!source) return;
		
		if (!source->isStreamed())
		{
			alSourcei(oalsource, AL_LOOPING, looped);
		}
		else
		{
		}
	}
	bool Sound::getLooped(void)
	{
		return false;
	}
	
	void Sound::setCleanUp(bool cleanup)
	{
		this->cleanup = cleanup;
	}
	bool Sound::getCleanUp(void)
	{
		return cleanup;
	}	
	
	void Sound::setPaused(bool paused)
	{
		if (oalsource)
		{
			// TODO: Is this correct?
			if (paused)
			{
				alSourcePause(oalsource);
			}
			else
			{
				alSourcePlay(oalsource);
			}
		}
	}
	bool Sound::isPaused(void)
	{
		if (oalsource)
		{
			int state = 0;
			alGetSourcei(oalsource, AL_SOURCE_STATE, &state);
			return (state != AL_PAUSED);
		}
		return false;
	}
	
	void Sound::setPlaying(bool playing)
	{
		if (!oalsource) return;
		if (!playing)
		{
			alSourceStop(oalsource);
		}
		else
		{
			alSourcePlay(oalsource);
		}
	}
	bool Sound::isPlaying(void)
	{
		if (oalsource)
		{
			int state = 0;
			alGetSourcei(oalsource, AL_SOURCE_STATE, &state);
			return (state != AL_STOPPED);
		}
		return false;
	}
	
	void Sound::setVolume(float volume)
	{
		alSourcef(oalsource, AL_GAIN, volume);
	}
	float Sound::getVolume(void)
	{
		float volume;
		alGetSourcef(oalsource, AL_GAIN, &volume);
		return volume;
	}
	
	void Sound::setPosition(Vector3D pos)
	{
		if (oalsource)
		{
			alSource3f(oalsource, AL_POSITION, pos.x, pos.y, pos.z);
		}
	}
	Vector3D Sound::getPosition(void)
	{
		if (oalsource)
		{
			Vector3D pos;
			alGetSource3f(oalsource, AL_POSITION, &pos.x, &pos.y, &pos.z);
			return pos;
		}
		return Vector3D(0, 0, 0);
	}
	void Sound::setVelocity(Vector3D vel)
	{
		if (oalsource)
		{
			alSource3f(oalsource, AL_VELOCITY, vel.x, vel.y, vel.z);
		}
	}
	Vector3D Sound::getVelocity(void)
	{
		if (oalsource)
		{
			Vector3D vel;
			alGetSource3f(oalsource, AL_POSITION, &vel.x, &vel.y, &vel.z);
			return vel;
		}
		return Vector3D(0, 0, 0);
	}
	
	void Sound::setRelativeToListener(bool rel)
	{
		if (oalsource)
		{
			alSourcei(oalsource, AL_SOURCE_RELATIVE, rel);
		}
	}
	bool Sound::isRelativeToListener(void)
	{
		if (oalsource)
		{
			int state = 0;
			alGetSourcei(oalsource, AL_SOURCE_RELATIVE, &state);
			return state;
		}
		return false;
	}
	
	void Sound::attachToSceneNode(SceneNode *node, bool adjustspeed)
	{
		this->node = node;
		this->adjustspeed = adjustspeed;
		if (oalsource)
		{
			if (node)
			{
				Vector3D pos = node->getPosition();
				alSource3f(oalsource, AL_POSITION, pos.x, pos.y, pos.z);
			}
			else
			{
				alSource3f(oalsource, AL_POSITION, 0, 0, 0);
			}
			alSource3f(oalsource, AL_VELOCITY, 0, 0, 0);
		}
	}
	
	void Sound::doWork(float msecs)
	{
		if (source && source->isStreamed())
		{
			((OggVorbisSource*)source)->update();
			// Refill processed buffers buffers
			int processed;
			alGetSourcei(oalsource, AL_BUFFERS_PROCESSED, &processed);
			for (int i = 0; i < processed; i++)
			{
				unsigned int buffer;
				alSourceUnqueueBuffers(oalsource, 1, &buffer);
				((OggVorbisSource*)source)->fillBuffer(buffer);
				alSourceQueueBuffers(oalsource, 1, &buffer);
			}
		}
		if (oalsource && node)
		{
			Vector3D pos = node->getPosition();
			alSource3f(oalsource, AL_POSITION, pos.x, pos.y, pos.z);
		}
	}
}


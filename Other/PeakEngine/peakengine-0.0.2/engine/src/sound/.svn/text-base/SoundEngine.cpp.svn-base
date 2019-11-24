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

#include "sound/SoundEngine.h"
#include "sound/Sound.h"
#include "sound/SoundFactory.h"
#include "sound/Listener.h"
#include "core/Logger.h"

#include <AL/al.h>
#include <AL/alut.h>
#include <math.h>

namespace peak
{
	SoundEngine *SoundEngine::get(void)
	{
		static SoundEngine engine;
		return &engine;
	}
	SoundEngine::~SoundEngine()
	{
	}
	
	bool SoundEngine::init(void)
	{
		if (!alutInit(0, 0))
		{
			LERROR("Could not initialize OpenAL.\n");
			return false;
		}
		
		initialized = true;
		listener = new Listener;
		return true;
	}
	bool SoundEngine::shutdown(void)
	{
		if (initialized)
		{
			// Delete sounds
			while (sounds.size() > 0)
			{
				delete *sounds.begin();
			}
			delete listener;
			listener = 0;
			// Close OpenAL
			alutExit();
			initialized = false;
		}
		return true;
	}
	bool SoundEngine::running(void)
	{
		return initialized;
	}
	
	Sound *SoundEngine::play2D(std::string filename, bool paused, bool looped, bool cleanup)
	{
		// Load source
		SoundSource *source = SoundFactory::get()->getFile(filename);
		if (!source)
		{
			return 0;
		}
		return play2D(source, paused, looped, cleanup);
	}
	Sound *SoundEngine::play2D(SoundSource *source, bool paused, bool looped, bool cleanup)
	{
		// Create sound
		Sound *newsound = new Sound;
		if (!newsound->init(source))
		{
			delete newsound;
			return 0;
		}
		
		// Modify settings
		newsound->setPaused(paused);
		newsound->setLooped(looped);
		newsound->setCleanUp(cleanup);
		newsound->setRelativeToListener(true);
		sounds.push_back(newsound);
		
		return newsound;
	}
	Sound *SoundEngine::play3D(std::string filename, bool paused, bool looped, bool cleanup)
	{
		// Load source
		SoundSource *source = SoundFactory::get()->getFile(filename);
		if (!source)
		{
			return 0;
		}
		return play3D(source, paused, looped, cleanup);
	}
	Sound *SoundEngine::play3D(SoundSource *source, bool paused, bool looped, bool cleanup)
	{
		// Create sound
		Sound *newsound = new Sound;
		if (!newsound->init(source))
		{
			delete newsound;
			return 0;
		}
		
		// Modify settings
		newsound->setPaused(paused);
		newsound->setLooped(looped);
		newsound->setCleanUp(cleanup);
		sounds.push_back(newsound);
		
		return newsound;
	}
	
	Listener *SoundEngine::getListener(void)
	{
		return listener;
	}
	
	void SoundEngine::doWork(float msecs)
	{
		// Loop through sounds
		std::list<Sound*>::iterator it = sounds.begin();
		while (it != sounds.end())
		{
			// Clean up data
			if ((*it)->getCleanUp() && !(*it)->isPlaying())
			{
				std::list<Sound*>::iterator nextit = it;
				nextit++;
				Sound *sound = *it;
				sounds.erase(it);
				delete sound;
				it = nextit;
				if (it == sounds.end()) break;
			}
			
			(*it)->doWork(msecs);
			it++;
		}
	}
	
	void SoundEngine::removeSoundFromList(Sound *sound)
	{
		// Search for sound
		std::list<Sound*>::iterator it = sounds.begin();
		while (it != sounds.end())
		{
			if (*it == sound)
			{
				// Delete sound from list
				sounds.erase(it);
				return;
			}
			it++;
		}
	}
	
	SoundEngine::SoundEngine()
	{
		listener = 0;
		initialized = false;
	}
}


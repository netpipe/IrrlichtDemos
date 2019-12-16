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

#include "sound/SoundFactory.h"
#include "sound/DataSoundSource.h"
#include "sound/OggVorbisSource.h"
#include "sound/SoundEngine.h"
#include "core/GameEngine.h"
#include "core/Util.h"
#include "core/FileSystem.h"

#include <AL/alut.h>

namespace peak
{
	SoundFactory *SoundFactory::get(void)
	{
		static SoundFactory factory;
		return &factory;
	}
	SoundFactory::~SoundFactory()
	{
	}
	
	SoundSource *SoundFactory::getFile(std::string filename, bool streamed)
	{
		if (!SoundEngine::get()->running()) return 0;
		// Search for already loaded file
		for (unsigned int i = 0; i < loadedfiles.size(); i++)
		{
			
			if (loadedfiles[i] == filename)
			{
				return loadedsources[i];
			}
		}
		// Load file
		std::string extension = getExtension(filename);
		LERROR("File %s, extension: %s\n", filename.c_str(), extension.c_str());
		if (extension == "ogg")
		{
			// Load ogg stream
			OggVorbisSource *oggsource = new OggVorbisSource;
			if (!oggsource->init(filename))
			{
				delete oggsource;
				return 0;
			}
			return oggsource;
		}
		else
		{
			// Load file using alut
			std::string file = FileSystem::get()->getFileExists(filename);
			unsigned int buffer = alutCreateBufferFromFile(file.c_str());
			if (buffer != AL_NONE)
			{
				DataSoundSource *newsource = new DataSoundSource;
				newsource->init(buffer);
				loadedsources.push_back(newsource);
				loadedfiles.push_back(filename);
				return newsource;
			}
			LERROR("Could not load wave source:\n");
			LERROR("%s\n", alutGetErrorString(alutGetError()));
			return 0;
		}
	}
	SoundSource *SoundFactory::getHelloWorld(void)
	{
		if (!SoundEngine::get()->running()) return 0;
		DataSoundSource *newsource = new DataSoundSource;
		newsource->init(alutCreateBufferHelloWorld());
		return newsource;
	}
	
	void SoundFactory::removeSource(SoundSource *source)
	{
		for (unsigned int i = 0; i < loadedfiles.size(); i++)
		{
			if (loadedsources[i] == source)
			{
				loadedsources.erase(loadedsources.begin() + i);
				loadedfiles.erase(loadedfiles.begin() + i);
				return;
			}
		}
	}
	
	SoundFactory::SoundFactory()
	{
	}
}


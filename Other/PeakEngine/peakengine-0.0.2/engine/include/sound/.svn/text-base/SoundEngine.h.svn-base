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

#ifndef _SOUNDENGINE_H_
#define _SOUNDENGINE_H_

#include <string>
#include <list>

//tolua_begin
namespace peak
{
	class Sound;
	class Listener;
	class SoundSource;
	
	/**
	 * \brief Class which sets up OpenAL and creates 2D and 3D sounds
	 */
	class SoundEngine
	{
		public:
			static SoundEngine *get(void);
			~SoundEngine();
			
			/**
			 * \brief Initializes the sound engine
			 */
			bool init(void);
			/**
			 * \brief Shuts down the sound engine
			 */
			bool shutdown(void);
			/**
			 * \brief Returns whether the sound engine was initialized
			 */
			bool running(void);
			
			/**
			 * \brief Plays a 2D sound from a file
			 * \param filename File name of the sound
			 * \param paused If set to true, the sound starts paused
			 * \param looped If set to true, the sound is looped
			 * \param cleanup If set to true, the sound is deleted after being played
			 */
			Sound *play2D(std::string filename, bool paused = false, bool looped = false, bool cleanup = true);
			/**
			 * \brief Plays a 2D sound from a sound source
			 * \param source SoundSource to use
			 * \param paused If set to true, the sound starts paused
			 * \param looped If set to true, the sound is looped
			 * \param cleanup If set to true, the sound is deleted after being played
			 */
			Sound *play2D(SoundSource *source, bool paused = false, bool looped = false, bool cleanup = true);
			/**
			 * \brief Plays a 3D sound from a file
			 *
			 * This only works with mono sound data!
			 * \param filename File name of the sound
			 * \param paused If set to true, the sound starts paused
			 * \param looped If set to true, the sound is looped
			 * \param cleanup If set to true, the sound is deleted after being played
			 */
			Sound *play3D(std::string filename, bool paused = false, bool looped = false, bool cleanup = true);
			/**
			 * \brief Plays a 3D sound from a sound source
			 *
			 * This only works with mono sound data!
			 * \param source SoundSource to use
			 * \param paused If set to true, the sound starts paused
			 * \param looped If set to true, the sound is looped
			 * \param cleanup If set to true, the sound is deleted after being played
			 */
			Sound *play3D(SoundSource *source, bool paused = false, bool looped = false, bool cleanup = true);
			
			/**
			 * \brief Returns the listener
			 */
			Listener *getListener(void);
			
			/**
			 * \brief Updates the sound engine, buffers strems etc.
			 */
			void doWork(float msecs);
			
			//tolua_end
			/**
			 * \brief Deletes a sound from the sound list
			 *
			 * For internal use only
			 */
			void removeSoundFromList(Sound *sound);
		private:
			SoundEngine();
			
			Listener *listener;
			
			std::list<Sound*> sounds;
			
			bool initialized;
			//tolua_begin
	};
}
//tolua_end

#endif


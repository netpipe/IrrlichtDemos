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

#ifndef _SOUNDFACTORY_H_
#define _SOUNDFACTORY_H_

#include <string>
#include <vector>

//tolua_begin
namespace peak
{
	class SoundSource;
	
	/**
	 * \brief Class which manages all sound sources
	 */
	class SoundFactory
	{
		public:
			static SoundFactory *get(void);
			~SoundFactory();
			
			/**
			 * \brief Gets the sound source from a file
			 * \param filename File to be read
			 * \param streamed If set to true, the sound source is streamed
			 */
			SoundSource *getFile(std::string filename, bool streamed = false);
			/**
			 * \brief Creates a sound source with a voice saying "Hello World"
			 */
			SoundSource *getHelloWorld(void);
			
			/**
			 * \brief Removes a sound source
			 *
			 * For internal use only, use drop() to delete sound sources.
			 */
			void removeSource(SoundSource *source);
		private:
			//tolua_end
			SoundFactory();
			
			std::vector<std::string> loadedfiles;
			std::vector<SoundSource*> loadedsources;
			//tolua_begin
	};
}
//tolua_end

#endif


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

#include "sound/SoundSource.h"

#ifndef _DATASOUNDSOURCE_H_
#define _DATASOUNDSOURCE_H_

//tolua_begin
namespace peak
{
	struct DataFormat
	{
	};
	
	/**
	 * \brief SoundSource which uses raw data
	 */
	class DataSoundSource : public SoundSource
	{
		public:
			DataSoundSource();
			virtual ~DataSoundSource();
			
			/**
			 * \brief Initializes the sound source based on the given data
			 * \param data Pointer to the data
			 * \param format Data format
			 * \param size Number of data frames
			 */
			bool init(void *data, DataFormat format, int size);
			/**
			 * \brief Initializes the sound source from an OpenAL buffer
			 * \param buffer OpenAL buffer
			 */
			bool init(unsigned int buffer);
			
			virtual int getLength(void);
			
			virtual bool isSeekable(void);
			virtual bool isStreamed(void);
			
			int getBuffer(void);
		private:
			//tolua_end
			unsigned int buffer;
			//tolua_begin
	};
};
//tolua_end

#endif


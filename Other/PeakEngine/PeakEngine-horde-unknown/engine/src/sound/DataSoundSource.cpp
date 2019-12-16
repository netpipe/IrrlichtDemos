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

#include "sound/DataSoundSource.h"
#include "sound/SoundFactory.h"

#include <AL/al.h>

namespace peak
{
	DataSoundSource::DataSoundSource()
	{
		buffer = 0;
	}
	DataSoundSource::~DataSoundSource()
	{
		SoundFactory::get()->removeSource(this);
		if (buffer)
		{
			alDeleteBuffers(1, &buffer);
		}
	}
	
	bool DataSoundSource::init(void *data, DataFormat format, int size)
	{
		return false;
	}
	bool DataSoundSource::init(unsigned int buffer)
	{
		this->buffer = buffer;
		return true;
	}
	
	int DataSoundSource::getLength(void)
	{
		return 0;
	}
	
	bool DataSoundSource::isSeekable(void)
	{
		return true;
	}
	bool DataSoundSource::isStreamed(void)
	{
		return false;
	}
	
	int DataSoundSource::getBuffer(void)
	{
		return buffer;
	}
}


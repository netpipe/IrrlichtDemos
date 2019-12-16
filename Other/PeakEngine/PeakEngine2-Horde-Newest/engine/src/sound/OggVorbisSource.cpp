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

#include "sound/OggVorbisSource.h"
#include "core/File.h"
#include "core/FileSystem.h"
#include "core/Logger.h"

#include <AL/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include <iostream>

#define BUFFER_SIZE (4096 * 8)

namespace peak
{
	// Wrapper functions to File class
	size_t vorbisReadCallback(void *buffer, size_t size, size_t count, void *fileptr)
	{
		File *file = (File*)fileptr;
		return file->read((char*)buffer, size * count);
	}
	int vorbisSeekCallback(void *fileptr, ogg_int64_t offset, int whence)
	{
		File *file = (File*)fileptr;
		if (whence == SEEK_SET)
		{
			return file->setPosition(offset);
		}
		else if (whence == SEEK_END)
		{
			return file->setPosition(offset, true);
		}
		else if (whence == SEEK_CUR)
		{
			return file->movePosition(offset);
		}
		return -1;
	}
	int vorbisCloseCallback(void *fileptr)
	{
		File *file = (File*)fileptr;
		delete file;
		return 1;
	}
	long vorbisTellCallback(void *fileptr)
	{
		File *file = (File*)fileptr;
		return file->getPosition();
	}
	
	OggVorbisSource::OggVorbisSource()
	{
		ovfile = 0;
	}
	OggVorbisSource::~OggVorbisSource()
	{
		if (ovfile)
		{
			ov_clear(ovfile);
			delete ovfile;
		}
	}
	
	bool OggVorbisSource::init(std::string filename)
	{
		LERROR("Loading file.\n");
		// Open file
		File *file = FileSystem::get()->openFile(filename, EFM_Read, true);
		if (!file)
		{
			LERROR("Could not open file \"%s\".\n", filename.c_str());
			return false;
		}
		
		// Load file
		ov_callbacks callbacks;
		callbacks.read_func = vorbisReadCallback;
		callbacks.seek_func = vorbisSeekCallback;
		callbacks.close_func = vorbisCloseCallback;
		callbacks.tell_func = vorbisTellCallback;
		ovfile = new OggVorbis_File;
		int result = ov_open_callbacks(file, ovfile, 0, 0, callbacks);
		if(result < 0)
		{
			LERROR("Could not load ogg stream \"%s\".\n", filename.c_str());
			delete ovfile;
			ovfile = 0;
			return false;
		}
		// Read file info
		vorbis_info *vorbisinfo = ov_info(ovfile, -1);
		vorbis_comment *comment = ov_comment(ovfile, -1);
		std::cout
		<< "version         " << vorbisinfo->version         << "\n"
		<< "channels        " << vorbisinfo->channels        << "\n"
		<< "rate (hz)       " << vorbisinfo->rate            << "\n"
		<< "bitrate upper   " << vorbisinfo->bitrate_upper   << "\n"
		<< "bitrate nominal " << vorbisinfo->bitrate_nominal << "\n"
		<< "bitrate lower   " << vorbisinfo->bitrate_lower   << "\n"
		<< "bitrate window  " << vorbisinfo->bitrate_window  << "\n"
		<< "\n"
		<< "vendor " << comment->vendor << "\n";
		for(int i = 0; i < comment->comments; i++)
		std::cout << "   " << comment->user_comments[i] << "\n";
		std::cout << std::endl;
		if (vorbisinfo->channels == 2)
		{
			stereo = true;
		}
		else
		{
			stereo = false;
		}
		rate = vorbisinfo->rate;
		return true;
	}
	
	int OggVorbisSource::getLength(void)
	{
		return 0;
	}
	
	bool OggVorbisSource::isSeekable(void)
	{
		return true;
	}
	bool OggVorbisSource::isStreamed(void)
	{
		return true;
	}
	
	bool OggVorbisSource::fillBuffer(unsigned int buffer)
	{
		if (!ovfile) return false;
		char data[BUFFER_SIZE];
		int  size = 0;
		int  section;
		int result;
		
		// Read data from file
		while(size < BUFFER_SIZE)
		{
			result = ov_read(ovfile, data + size, BUFFER_SIZE - size, 0, 2, 1, &section);
			if (result == 0)
			{
				break;
			}
			else if (result < 0)
			{
				LERROR("Error while streaming data.\n");
				return false;
			}
			size += result;
		}
		// Buffer data
		if (stereo)
		{
			alBufferData(buffer, AL_FORMAT_STEREO16, data, size, rate);
		}
		else
		{
			alBufferData(buffer, AL_FORMAT_MONO16, data, size, rate);
		}
		
		return true;
	}
	
	void OggVorbisSource::update(void)
	{
	}
}


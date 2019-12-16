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

#include "core/File.h"
#include "core/Buffer.h"
#include <stdio.h>
#include <cstring>

namespace peak
{
	File::File(std::string filename, EFileMode mode, bool binary)
	{
		char modestr[4];
		switch (mode)
		{
			case EFM_Read:
				strcpy(modestr, "r");
				break;
			case EFM_Write:
				strcpy(modestr, "w");
				break;
			case EFM_ReadWrite:
				strcpy(modestr, "w+");
				break;
			case EFM_Append:
				strcpy(modestr, "a");
				break;
			case EFM_ReadAppend:
				strcpy(modestr, "a+");
				break;
			default:
				strcpy(modestr, "r");
				break;
		}
		if (binary)
		{
			modestr[strlen(modestr) + 1] = 0;
			modestr[strlen(modestr)] = 'b';
		}
		file = fopen(filename.c_str(), modestr);
	}
	File::~File()
	{
		if (file)
		{
			fclose(file);
		}
	}

	int File::read(char *buffer, int length)
	{
		return fread(buffer, 1, length, file);
	}
	int File::read(Buffer *buffer, int length)
	{
		char *tmpbuffer = new char[length];
		int bytesread = fread(tmpbuffer, 1, length, file);
		buffer->writeData(tmpbuffer, bytesread, true);
		delete[] tmpbuffer;
		return bytesread;
	}
	std::string File::readLine(void)
	{
		char data[100];
		if (!fgets(data, 100, file))
		{
			return "";
		}
		if (data[strlen(data) - 1] == '\n')
		{
			data[strlen(data) - 1] = 0;
		}
		return data;
	}
	int File::write(const char *buffer, int length)
	{
		return fwrite(buffer, length, 1, file);
	}
	int File::write(Buffer *buffer, int length)
	{
		return fwrite(buffer->getData(), buffer->getDataSize(), 1, file);
	}
	bool File::eof(void)
	{
		return !feof(file);
	}

	int File::getPosition(void)
	{
		return ftell(file);
	}
	int File::setPosition(int pos, bool end)
	{
		if (!end)
		{
			fseek(file, pos, SEEK_SET);
		}
		else
		{
			fseek(file, pos, SEEK_END);
		}
		return getPosition();
	}
	int File::movePosition(int pos)
	{
		fseek(file, pos, SEEK_CUR);
		return getPosition();
	}
}


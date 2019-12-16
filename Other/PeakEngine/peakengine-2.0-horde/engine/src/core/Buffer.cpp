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

#include "core/Buffer.h"

#include <stdlib.h>
#include <string.h>

namespace peak
{
	Buffer::Buffer()
	{
		data = 0;
		size = 0;
		position = 0;
		datasize = 0;
	}
	Buffer::Buffer(unsigned int size)
	{
		data = (char*)malloc(size);
		size = size;
		position = 0;
		datasize = 0;
	}
	Buffer::Buffer(void *data, unsigned int size)
	{
		this->data = (char*)malloc(size);
		size = size;
		datasize = size;
		position = 0;
		memcpy(this->data, data, size);
	}
	Buffer::Buffer(const Buffer &buf)
	{
		data = 0;
		size = 0;
		datasize = 0;
		position = 0;
		*this += buf;
	}
	Buffer::~Buffer()
	{
		if (data)
			free(data);
	}

	void Buffer::setSize(unsigned int size)
	{
		data = (char*)realloc(data, size);
		if (datasize > size)
			datasize = size;
		if (position > datasize)
			position = datasize;
		this->size = size;

	}
	unsigned int Buffer::getSize(void)
	{
		return size;
	}
	unsigned int Buffer::getDataSize(void)
	{
		return datasize;
	}

	unsigned int Buffer::getPosition(void)
	{
		return position;
	}
	void Buffer::setPosition(int position, bool relative)
	{
		if (!relative)
		{
			if (position < 0) position = 0;
			if (position > (int)datasize)
				position = datasize;
			this->position = position;
		}
		else
		{
			if (this->position + position > datasize)
				position = datasize - this->position;
			if ((int)this->position + position < 0)
				position = -this->position;
			this->position = (int)this->position + position;
		}
	}

	int Buffer::readData(void *dest, unsigned int size)
	{
		if (datasize - position < size)
		{
			size = datasize - position;
		}
		memcpy(dest, data + position, size);
		position += size;
		return size;
	}
	int Buffer::writeData(const void *source, unsigned int size, bool resize)
	{
		if (this->size - datasize < size)
		{
			if (resize)
			{
				setSize(datasize + size);
			}
			else
			{
				size = this->size - datasize;
			}
		}
		memcpy(data + datasize, source, size);
		datasize += size;
		return size;
	}

	void Buffer::writeInt(int value)
	{
		writeData(&value, sizeof(int), true);
	}
	int Buffer::readInt(void)
	{
		int value;
		readData(&value, sizeof(int));
		return value;
	}
	void Buffer::writeFloat(float value)
	{
		writeData(&value, sizeof(float), true);
	}
	float Buffer::readFloat(void)
	{
		float value;
		readData(&value, sizeof(float));
		return value;
	}
	void Buffer::writeString(std::string str)
	{
		writeData(str.c_str(), str.length() + 1, true);
	}
	std::string Buffer::readString(void)
	{
		std::string str = data + position;
		while (readByte())
			;
		return str;
	}
	void Buffer::writeByte(unsigned char value)
	{
		writeData(&value, sizeof(unsigned char), true);
	}
	unsigned char Buffer::readByte(void)
	{
		unsigned char value;
		readData(&value, sizeof(unsigned char));
		return value;
	}
	void Buffer::writeWord(short value)
	{
		writeData(&value, sizeof(short), true);
	}
	short Buffer::readWord(void)
	{
		short value;
		readData(&value, sizeof(short));
		return value;
	}
	void Buffer::writeBool(bool flag)
	{
		writeByte(flag);
	}
	bool Buffer::readBool(void)
	{
		bool flag = readByte();
		return flag;
	}
	void Buffer::writeVector2D(Vector2D v)
	{
		writeFloat(v.x);
		writeFloat(v.y);
	}
	Vector2D Buffer::readVector2D(void)
	{
		Vector2D v;
		v.x = readFloat();
		v.y = readFloat();
		return v;
	}
	void Buffer::writeVector3D(Vector3D v)
	{
		writeFloat(v.x);
		writeFloat(v.y);
		writeFloat(v.z);
	}
	Vector3D Buffer::readVector3D(void)
	{
		Vector3D v;
		v.x = readFloat();
		v.y = readFloat();
		v.z = readFloat();
		return v;
	}
	void Buffer::writeQuaternion(Quaternion q)
	{
		writeWord((unsigned short)(q.x * (1<<15)));
		writeWord((unsigned short)(q.y * (1<<15)));
		writeWord((unsigned short)(q.z * (1<<15)));
		writeWord((unsigned short)(q.w * (1<<15)));
	}
	Quaternion Buffer::readQuaternion(void)
	{
		Quaternion q;
		q.x = (float)readWord() / (1<<15);
		q.y = (float)readWord() / (1<<15);
		q.z = (float)readWord() / (1<<15);
		q.w = (float)readWord() / (1<<15);
		return q;
	}


	char *Buffer::getData(void)
	{
		return data;
	}

	void Buffer::clear(void)
	{
		if (data)
			free(data);
		data = 0;
		size = 0;
		datasize = 0;
		position = 0;
	}

	Buffer &Buffer::operator=(const Buffer &buf)
	{
		clear();
		*this += buf;
		return *this;
	}
	Buffer &Buffer::operator+=(const Buffer &buf)
	{
		writeData(buf.data, buf.datasize, true);
		return *this;
	}
	Buffer &Buffer::operator<<(const Buffer &buf)
	{
		writeData(buf.data, buf.datasize, true);
		return *this;
	}
	Buffer &Buffer::operator<<(int &data)
	{
		writeData(&data, sizeof(int), true);
		return *this;
	}
	Buffer &Buffer::operator<<(float &data)
	{
		writeData(&data, sizeof(float), true);
		return *this;
	}
}


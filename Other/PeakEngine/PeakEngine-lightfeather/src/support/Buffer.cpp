/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakengine/support/Buffer.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

namespace peak
{
	static inline unsigned int bytes(unsigned int bits)
	{
		return (bits + 7) / 8;
	}
	Buffer::Buffer() : ReferenceCounted()
	{
		data = 0;
		size = 0;
		position = 0;
	}
	Buffer::Buffer(void *data, unsigned int size, bool copy) : ReferenceCounted()
	{
		if (copy)
		{
			this->data = (char*)malloc(size);
			memcpy(this->data, data, size);
			this->size = size;
		}
		else
		{
			this->data = (char*)data;
			this->size = size;
		}
		position = 0;
	}
	Buffer::Buffer(const Buffer &b) : ReferenceCounted()
	{
		data = (char*)malloc(b.size);
		memcpy(data, b.data, b.size);
		size = b.size;
		position = 0;
	}
	Buffer::~Buffer()
	{
		if (data)
			free(data);
	}

	void Buffer::setSize(unsigned int size)
	{
		data = (char*)realloc(data, size);
		this->size = size;
		if (position > size)
			position = size;
	}
	unsigned int Buffer::getSize()
	{
		return size;
	}

	void Buffer::setPosition(unsigned int position)
	{
		this->position = position;
		if (position > size * 8)
			this->position = size * 8;
	}
	unsigned int Buffer::getPosition()
	{
		return position;
	}

	void *Buffer::getData()
	{
		return data;
	}

	void Buffer::write8(uint8_t value)
	{
		if (position % 8)
		{
			if (position + 8 > size * 8)
			{
				data = (char*)realloc(data, size + 1);
				size++;
			}
			writeByte(value);
			position += 8;
		}
		else
		{
			// We are on an even position
			if (position / 8 == size)
			{
				data = (char*)realloc(data, size + 1);
				*(uint8_t*)(data + position / 8) = value;
				size++;
				position += 8;
			}
			else
			{
				*(uint8_t*)(data + position / 8) = value;
				position++;
			}
		}
	}
	uint8_t Buffer::read8()
	{
		if (position + 8 > size * 8)
		{
			position = size * 8;
			return 0;
		}
		if (position % 8)
		{
			uint8_t byte = readByte();
			position += 8;
			if (position > size * 8)
				position = size * 8;
			return byte;
		}
		else
		{
			// We are on an even position
			uint8_t value = *(uint8_t*)(data + position / 8);
			position += 8;
			return value;
		}
	}
	void Buffer::write16(uint16_t value)
	{
		if (position % 8)
		{
			if (position + 16 > size * 8)
			{
				data = (char*)realloc(data, bytes(position + 16));
				size = bytes(position + 16);
			}
			writeByte(value >> 8);
			position += 8;
			writeByte(value & 0xFF);
			position += 8;
		}
		else
		{
			// We are on an even position
			if (position / 8 + 2 > size)
			{
				data = (char*)realloc(data, position / 8 + 2);
				*(uint16_t*)(data + position / 8) = htons(value);
				position += 16;
				size = position / 8;
			}
			else
			{
				*(uint16_t*)(data + position / 8) = htons(value);
				position += 16;
			}
		}
	}
	uint16_t Buffer::read16()
	{
		if (position + 16 > size * 8)
		{
			position = size * 8;
			return 0;
		}
		if (position % 8)
		{
			uint16_t value = readByte() << 8;
			position += 8;
			if (position > size * 8)
				position = size * 8;
			value |= readByte();
			position += 8;
			if (position > size * 8)
				position = size * 8;
			return value;
		}
		else
		{
			// We are on an even position
			uint16_t value = *(uint16_t*)(data + position / 8);
			position += 16;
			return ntohs(value);
		}
	}
	void Buffer::write32(uint32_t value)
	{
		if (position % 8)
		{
			if (position + 32 > size * 8)
			{
				data = (char*)realloc(data, bytes(position + 32));
				size = bytes(position + 32);
			}
			for (int shift = 24; shift >= 0; shift -= 8)
			{
				writeByte((value >> shift) & 0xFF);
				position += 8;
			}
		}
		else
		{
			// We are on an even position
			if (position / 8 + 4 > size)
			{
				data = (char*)realloc(data, position / 8 + 4);
				*(uint32_t*)(data + position / 8) = htonl(value);
				position += 32;
				size += 4;
			}
			else
			{
				*(uint32_t*)(data + position / 8) = htonl(value);
				position += 32;
			}
		}
	}
	uint32_t Buffer::read32()
	{
		if (position + 32 > size * 8)
		{
			position = size * 8;
			return 0;
		}
		if (position % 8)
		{
			uint32_t value = 0;
			for (int shift = 24; shift >= 0; shift -= 8)
			{
				value |= (uint32_t)readByte() << shift;
				position += 8;
				if (position > size * 8)
					position = size * 8;
			}
			return value;
		}
		else
		{
			// We are on an even position
			uint32_t value = *(uint32_t*)(data + position / 8);
			position += 32;
			return ntohl(value);
		}
	}
	void Buffer::write64(uint64_t value)
	{
		if (position % 8)
		{
			if (position + 64 > size * 8)
			{
				data = (char*)realloc(data, bytes(position + 64));
				size = bytes(position + 64);
			}
			for (int shift = 56; shift >= 0; shift -= 8)
			{
				writeByte((value >> shift) & 0xFF);
				position += 8;
			}
		}
		else
		{
			// We are on an even position
			// TODO: Byte ordering
			if (position / 8 + 8 > size)
			{
				data = (char*)realloc(data, position / 8 + 8);
				*(uint64_t*)(data + position / 8) = value;
				position += 64;
				size += 8;
			}
			else
			{
				*(uint64_t*)(data + position / 8) = value;
				position += 64;
			}
		}
	}
	uint64_t Buffer::read64()
	{
		if (position + 64 > size * 8)
		{
			position = size * 8;
			return 0;
		}
		if (position % 8)
		{
			uint32_t value = 0;
			for (int shift = 56; shift >= 0; shift -= 8)
			{
				value |= (uint64_t)readByte() << shift;
				position += 8;
				if (position > size * 8)
					position = size * 8;
			}
			return value;
		}
		else
		{
			// We are on an even position
			// TODO: Byte ordering
			uint64_t value = *(uint64_t*)(data + position / 8);
			position += 64;
			return value;
		}
	}

	void Buffer::writeFloat(float value)
	{
		unsigned int intvalue;
		*(float*)&intvalue = value;
		write32(intvalue);
	}
	float Buffer::readFloat()
	{
		unsigned int intvalue = read32();
		return *(float*)&intvalue;
	}

	void Buffer::writeString(std::string value)
	{
		if (position % 8)
		{
			const char *str = value.c_str();
			for (unsigned int i = 0; i < value.size() + 1; i++)
			{
				write8(str[i]);
			}
		}
		else
		{
			// We are on an even position
			if (position / 8 + value.size() + 1 > size)
			{
				data = (char*)realloc(data, position / 8 + value.size() + 1);
				strcpy(data + position / 8, value.c_str());
				position += (value.size() + 1) * 8;
				size += value.size() + 1;
			}
			else
			{
				strcpy(data + position / 8, value.c_str());
				position += (value.size() + 1) * 8;
			}
		}
	}
	std::string Buffer::readString()
	{
		if (position % 8)
		{
			std::string value;
			char c = read8();
			while (c)
			{
				value += c;
				c = read8();
			}
			return value;
		}
		else
		{
			// We are on an even position
			if (position / 8 == size)
				return "";
			std::string value;
			while (data[position / 8] != 0)
			{
				value += data[position / 8];
				position += 8;
				if (position / 8 == size)
					break;
			}
			if (position / 8 < size)
				position += 8;
			return value;
		}
	}

	void Buffer::write(void *data, unsigned int size)
	{
		char *cdata = (char*)data;
		if (position % 8)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				write8(cdata[i]);
			}
		}
		else
		{
			// We are on an even position
			if (position / 8 + size > this->size)
			{
				this->data = (char*)realloc(this->data, position / 8 + size);
				memcpy(this->data + position / 8, cdata, size);
				position += size * 8;
				this->size += size;
			}
			else
			{
				memcpy(this->data + position / 8, cdata, size);
				position += size * 8;
			}
		}
	}
	unsigned int Buffer::read(void *data, unsigned int size)
	{
		char *cdata = (char*)data;
		if (position % 8)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				cdata[i] = read8();
			}
			// TODO: Return length
			return size;
		}
		else
		{
			// We are on an even position
			if (position / 8 == this->size)
				return 0;
			char *cdata = (char*)data;
			for (unsigned int i = 0; i < size; i++)
			{
				cdata[i] = this->data[position / 8];
				position += 8;
				if (position / 8 == this->size)
					return i + 1;
			}
			if (position / 8 < this->size)
				position += 8;
			return size;
		}
	}

	void Buffer::writeInt(int value, unsigned int size)
	{
		writeUnsignedInt(value, size);
	}
	int Buffer::readInt(unsigned int size)
	{
		unsigned int value = 0;
		for (unsigned int i = 0; i < (size + 7) / 8; i++)
		{
			((char*)&value)[i] = readByte();
			if (size - i * 8 >= 8)
				position += 8;
			else
			{
				position += size - i * 8;
				((char*)&value)[i] &= 0xFF << (((size + 7) & ~7) - size);
			}
		}
		if (position > this->size * 8)
			position = this->size * 8;
		value = ntohl(value);
		// Extend sign if necessary
		if (value & 0x80000000)
			return (value >> (32 - size)) | (0xFFFFFFFF << size);
		else
			return value >> (32 - size);
	}
	void Buffer::writeUnsignedInt(unsigned int value, unsigned int size)
	{
		// Allocate memory
		if (position + size > this->size * 8)
		{
			data = (char*)realloc(data, bytes(position + size));
			this->size = bytes(position + size);
		}
		// Left-align the number in memory
		value <<= 32 - size;
		value = htonl(value);
		for (unsigned int i = 0; i < (size + 7) / 8; i++)
		{
			writeByte(((char*)&value)[i]);
			if (size - i * 8 >= 8)
				position += 8;
			else
				position += size - i * 8;
		}
	}
	unsigned int Buffer::readUnsignedInt(unsigned int size)
	{
		unsigned int value = 0;
		for (unsigned int i = 0; i < (size + 7) / 8; i++)
		{
			((char*)&value)[i] = readByte();
			if (size - i * 8 >= 8)
				position += 8;
			else
			{
				position += size - i * 8;
				((char*)&value)[i] &= 0xFF << (((size + 7) & ~7) - size);
			}
		}
		if (position > this->size * 8)
			position = this->size * 8;
		value = ntohl(value);
		return value >> (32 - size);
	}

	void Buffer::nextByte()
	{
		position = (position + 7) & ~7;
	}

	Buffer &Buffer::operator=(const Buffer &b)
	{
		data = (char*)malloc(b.size);
		memcpy(data, b.data, b.size);
		size = b.size;
		position = 0;
		return *this;
	}
	Buffer &Buffer::operator+=(const Buffer &b)
	{
		// TODO: Non-aligned buffers
		data = (char*)realloc(data, size + b.size);
		memcpy(data + size, b.data, b.size);
		size += b.size;
		return *this;
	}

	void Buffer::dump()
	{
		char *msg = (char*)malloc(size * 3);
		for (unsigned int i = 0; i < size; i++)
		{
			#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
			_snprintf(msg + i * 3, (size - i) * 3, "%02x ", (unsigned char)data[i]);
			#else
			snprintf(msg + i * 3, (size - i) * 3, "%02x ", (unsigned char)data[i]);
			#endif
		}
		std::cout << msg << std::endl;
		free(msg);
	}

	void Buffer::writeByte(unsigned char value)
	{
		if (position % 8)
		{
			// Write two halves of the byte
			int offset = position % 8;
			unsigned char b1 = value >> offset;
			unsigned char b2 = value << (8 - offset);
			unsigned char mask1 = 0xFF >> offset;
			unsigned char mask2 = 0xFF << (8 - offset);
			data[position / 8] &= mask2;
			data[position / 8] |= b1;
			if (position / 8 + 1 < size)
			{
				data[position / 8 + 1] &= mask1;
				data[position / 8 + 1] |= b2;
			}
		}
		else
		{
			// Directly write one byte
			data[position / 8] = value;
		}
	}
	unsigned char Buffer::readByte()
	{
		if (position >= size * 8)
			return 0;
		if (position % 8)
		{
			int offset = position % 8;
			if (size * 8 < position + 8)
			{
				// Only read one half
				unsigned char b = data[position / 8];
				b <<= offset;
				return b;
			}
			else
			{
				// Read the whole byte
				unsigned char b1 = data[position / 8];
				unsigned char b2 = data[position / 8 + 1];
				return (b1 << offset) | (b2 >> (8 - offset));
			}
		}
		else
		{
			return data[position / 8];
		}
	}
}

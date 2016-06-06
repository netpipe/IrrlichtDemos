#include <stdio.h>
#include <NxPhysics.h>
#include "PhysXStream.h"

PhysXUserStream::PhysXUserStream(const char* filename, bool load) : fp(NULL)
	{
	fp = fopen(filename, load ? "rb" : "wb");
	}

PhysXUserStream::~PhysXUserStream()
	{
	if(fp)	fclose(fp);
	}

// Loading API
NxU8 PhysXUserStream::readByte() const
	{
	NxU8 b;
	size_t r = fread(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(r);
	return b;
	}

NxU16 PhysXUserStream::readWord() const
	{
	NxU16 w;
	size_t r = fread(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(r);
	return w;
	}

NxU32 PhysXUserStream::readDword() const
	{
	NxU32 d;
	size_t r = fread(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(r);
	return d;
	}

float PhysXUserStream::readFloat() const
	{
	NxReal f;
	size_t r = fread(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(r);
	return f;
	}

double PhysXUserStream::readDouble() const
	{
	NxF64 f;
	size_t r = fread(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(r);
	return f;
	}

void PhysXUserStream::readBuffer(void* buffer, NxU32 size)	const
	{
	size_t w = fread(buffer, size, 1, fp);
	NX_ASSERT(w);
	}

// Saving API
NxStream& PhysXUserStream::storeByte(NxU8 b)
	{
	size_t w = fwrite(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(w);
	return *this;
	}

NxStream& PhysXUserStream::storeWord(NxU16 w)
	{
	size_t ww = fwrite(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(ww);
	return *this;
	}

NxStream& PhysXUserStream::storeDword(NxU32 d)
	{
	size_t w = fwrite(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(w);
	return *this;
	}

NxStream& PhysXUserStream::storeFloat(NxReal f)
	{
	size_t w = fwrite(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(w);
	return *this;
	}

NxStream& PhysXUserStream::storeDouble(NxF64 f)
	{
	size_t w = fwrite(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(w);
	return *this;
	}

NxStream& PhysXUserStream::storeBuffer(const void* buffer, NxU32 size)
	{
	size_t w = fwrite(buffer, size, 1, fp);
	NX_ASSERT(w);
	return *this;
	}




PhysXMemoryWriteBuffer::PhysXMemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
	{
	}

PhysXMemoryWriteBuffer::~PhysXMemoryWriteBuffer()
	{
	NX_DELETE_ARRAY(data);
	}

void PhysXMemoryWriteBuffer::clear()
	{
	currentSize = 0;
	}

NxStream& PhysXMemoryWriteBuffer::storeByte(NxU8 b)
	{
	storeBuffer(&b, sizeof(NxU8));
	return *this;
	}
NxStream& PhysXMemoryWriteBuffer::storeWord(NxU16 w)
	{
	storeBuffer(&w, sizeof(NxU16));
	return *this;
	}
NxStream& PhysXMemoryWriteBuffer::storeDword(NxU32 d)
	{
	storeBuffer(&d, sizeof(NxU32));
	return *this;
	}
NxStream& PhysXMemoryWriteBuffer::storeFloat(NxReal f)
	{
	storeBuffer(&f, sizeof(NxReal));
	return *this;
	}
NxStream& PhysXMemoryWriteBuffer::storeDouble(NxF64 f)
	{
	storeBuffer(&f, sizeof(NxF64));
	return *this;
	}
NxStream& PhysXMemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
	{
	NxU32 expectedSize = currentSize + size;
	if(expectedSize > maxSize)
		{
		maxSize = expectedSize + 4096;

		NxU8* newData = new NxU8[maxSize];
		NX_ASSERT(newData!=NULL);

		if(data)
			{
			memcpy(newData, data, currentSize);
			delete[] data;
			}
		data = newData;
		}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
	}


PhysXMemoryReadBuffer::PhysXMemoryReadBuffer(const NxU8* data) : buffer(data)
	{
	}

PhysXMemoryReadBuffer::~PhysXMemoryReadBuffer()
	{
	// We don't own the data => no delete
	}

NxU8 PhysXMemoryReadBuffer::readByte() const
	{
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
	}

NxU16 PhysXMemoryReadBuffer::readWord() const
	{
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
	}

NxU32 PhysXMemoryReadBuffer::readDword() const
	{
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
	}

float PhysXMemoryReadBuffer::readFloat() const
	{
	float f;
	memcpy(&f, buffer, sizeof(float));
	buffer += sizeof(float);
	return f;
	}

double PhysXMemoryReadBuffer::readDouble() const
	{
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
	}

void PhysXMemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
	{
	memcpy(dest, buffer, size);
	buffer += size;
	}

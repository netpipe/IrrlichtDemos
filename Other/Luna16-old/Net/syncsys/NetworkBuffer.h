#ifdef SYNCSYS
#pragma once

#ifndef DONT_INCLUDE_HEADERS
#include "Portable.h"
#include <string>
#endif

//! namespace for all useful classes/functions etc.
namespace helpers
{
	enum Allocstrategy { BUFFER_ADD, BUFFER_DOUBLE };
//!helpclass for data-(de)-serialization.
class NetworkBuffer
{
	char			staticbuffer[1024];
	char*			buffer;
	unsigned		size;
	unsigned		roffset;
	unsigned		woffset;
	Allocstrategy	strategy;

	NetworkBuffer(const NetworkBuffer&);

public:
	//!standard c'tor
	NetworkBuffer(Allocstrategy strat = BUFFER_ADD)
		: buffer(staticbuffer), size(sizeof(staticbuffer)), roffset(0), woffset(0), strategy(strat)	{ }
	//!c'tor which calls CheckUnusedSize(_size).
	NetworkBuffer(unsigned _size, Allocstrategy strat = BUFFER_ADD)
		: buffer(staticbuffer), size(sizeof(staticbuffer)), roffset(0), woffset(0), strategy(strat)	{ CheckUnusedSize(_size); }
	~NetworkBuffer(void)																			{ if(buffer != staticbuffer) delete[] buffer; }

	//!set the readoffset, which must not be greater then the writeoffset.
	void		SetReadOffset(unsigned newoffset)		{ _ASSERT(newoffset <= woffset); roffset = newoffset; }
	//!set the Writeoffset, which must not be greater then the buffersize.
	void		SetWriteOffset(unsigned newoffset)	{ _ASSERT(newoffset <= size); woffset = newoffset; }
	//!set the read- and the writeoffset to zero.
	void		ResetOffsets(void)						{ roffset = 0; woffset = 0; }
	//!getter for the readoffset
	unsigned	GetReadOffset(void) const				{ return roffset; }
	//!getter for the writeoffset
	unsigned	GetWriteOffset(void) const				{ return woffset; }
	//!getter for buffersize - writeoffset = unusedsize
	unsigned	GetUnusedSize(void) const				{ return size - woffset; }
	//!getter for writeoffset - readoffset = unreadsize
	unsigned	GetUnreadSize(void) const				{ return woffset - roffset; }
	//!getter for buffersize
	unsigned	GetBufferSize(void) const				{ return size; }
	//!getter for direct access to the buffer. Internal purpose only.
	const char*	GetData(void) const						{ return buffer; }
	//!getter for direct access with pointershift (buffer + writeoffset)
	const char*	GetDataFromWOffset(void) const			{ return buffer + woffset; }
	//!getter for direct access with pointershift (buffer + readoffset)
	const char*	GetDataFromROffset(void) const			{ return buffer + roffset; }

	void operator = (const NetworkBuffer &c)
	{
		CheckBufferSize(c.woffset, 0);
		memcpy(buffer, c.buffer, c.woffset);
		woffset	= c.woffset;
		roffset	= c.roffset;
	}
	//!compares two buffers with memcmp. If ignoreOffsets is false, the ReadOffset is used.
	bool EqualBuffers(const NetworkBuffer &_buf, bool ignoreOffsets)
	{
		if(ignoreOffsets)
			return(GetBufferSize() == _buf.GetBufferSize() && 0 == memcmp(buffer, _buf.GetData(), GetBufferSize()));
		else
			return(GetUnreadSize() == _buf.GetUnreadSize() && 0 == memcmp(GetDataFromROffset(), _buf.GetDataFromROffset(), GetUnreadSize()));
	}
	//!Copy data from buf directly into buffer.
	/** \note Realloc buffer, if buffer is undersized.*/
	void SetDaten(const char* buf, unsigned newsize)
	{
		CheckBufferSize(newsize, 0);

		memcpy(buffer, buf, newsize);
		woffset = newsize;
		roffset = 0;
	}
	//!force the buffer to resize (for example if it takes to much space). Shift offsets as well.
	void ResizeBuffer(unsigned newsize, bool copydata, unsigned copyoffset = 0)
	{
		char* temp = NULL;
		//if staticbuffer is not large enought
		if(newsize > sizeof(staticbuffer))
			temp = new char[newsize];
		else
		{
			temp	= staticbuffer;
			newsize	= sizeof(staticbuffer);
		}

		if(copydata)
		{
			_ASSERT(woffset + copyoffset <= newsize);
			memcpy(temp + copyoffset, buffer, woffset);
			woffset += copyoffset;
			roffset += copyoffset;
		}
		else
		{
			woffset = 0;
			roffset = 0;
		}

		if(buffer != staticbuffer)	delete[] buffer;

		buffer		= temp;
		size		= newsize;
	}
	//!Checks that there is enough free space.
	/**return true, if unusedsize is greate enought. Else it resizes the buffer to fit and return false.*/
	bool CheckUnusedSize(unsigned neededsize)
	{
		return CheckBufferSize(woffset + neededsize, 0);
	}
	//!Checks that buffer is big enought.
	/**return true, if buffersize is greate enought. Else it resizes the buffer to fit and return false. Only copy with offset (shift to right) if it has to resize.*/
	bool CheckBufferSize(unsigned neededsize, unsigned copyoffset)
	{
		if(neededsize <= size)
			return true;

		unsigned newsize = size;

		if(strategy == BUFFER_ADD)	newsize = neededsize;
		else						while(newsize < neededsize) newsize *= 2;

		ResizeBuffer(newsize, true, copyoffset);
		return false;
	}
	//!shift the data and offsets by numbytes, to left or to right
	void ShiftBuffer(bool left, unsigned numbytes)
	{
		_ASSERT(buffer);
		if(!left)
		{
			//extends buffer
			if(CheckBufferSize(woffset + numbytes, numbytes))
				memmove(buffer + numbytes, buffer, woffset);
			woffset += numbytes;
			roffset += numbytes;
		}
		else
		{
			if(woffset <= numbytes)
			{
				woffset = 0;
				roffset = 0;
			}
			else
			{
				woffset -= numbytes;
				roffset -= numbytes;
				memmove(buffer, buffer + numbytes, woffset);
			}
		}
	}

	//!void*-buffer write methode.
	/**Write _buffer direct to buffer, without writing _size.
	   \note Realloc buffer, if buffer is undersized.*/
	void WritePlain(volatile const void* _buffer, unsigned _size)
	{
		CheckUnusedSize(_size);
		memcpy(buffer + woffset, const_cast<const void*>(_buffer), _size);	woffset += _size;
	}
	//!char*-string write methode.
	/**string should be nullterminated. Measure length, write length(unsigned int) + string(char*).
	\note Realloc buffer, if buffer is to small.*/
	void Write(const char* string)
	{
		unsigned int _size = static_cast<unsigned int>(strlen(string) + 1);

		CheckUnusedSize(_size + sizeof(int));

		memcpy(buffer + woffset, &_size, sizeof(int));	woffset += sizeof(int);
		memcpy(buffer + woffset, string, _size);		woffset += _size;
	}
	//!wchar_t*-string write methode.
	/**string should be nullterminated. Convert to multibyte, measure length, write length(unsigned int) + string(char*).
	\note Realloc buffer, if buffer is to small.*/
	void Write(const wchar_t* string)
	{
		unsigned int _size = static_cast<unsigned int>(wcs2mbslen(string) + 1);

		CheckUnusedSize(_size + sizeof(int));

		memcpy(buffer + woffset, &_size, sizeof(int));	woffset += sizeof(int);
		wcs2mbs_n(buffer + woffset, string, _size);		woffset += _size;
	}
	//!template write methode.
	/** \attention Do not use for pointer!
	\note Set WriteOffset to offset, write data and reset ReadOffset to old value*/
	template<class T1> void WriteAt(const volatile T1 &t, unsigned offset)
	{
		unsigned oldwoffset	= woffset;
		woffset				= offset;
		Write(t);
		woffset				= oldwoffset;
	}
	//!Appends _buffer to this buffer.
	/** Readoffset of _buf is used. \note Realloc buffer, if buffer is undersized.*/
	void Write(const NetworkBuffer& _buf)
	{
		_ASSERT(this != &_buf);
		WritePlain(_buf.GetDataFromROffset(), _buf.GetUnreadSize());
	}
	//!redirector (template).
	template<class T1> void Write(const volatile T1 &t)	{ WritePlain(const_cast<const T1*>(&t), sizeof(t)); }
	//!redirector (template).
	template<class T1> void Write(const T1 &t)	{ WritePlain(const_cast<const T1*>(&t), sizeof(t)); }
	//!redirector for std::string.
	void Write(const std::string &str)					{ Write(str.c_str()); }
	//!redirector for std::wstring.
	void Write(const std::wstring &str)					{ Write(str.c_str()); }
	//!redirector for char* strings (has to be nullterminated)
	void Write(char* string)							{ Write(static_cast<const char*>(string)); }
	//!redirector for wchar_t* strings (has to be nullterminated)
	void Write(wchar_t* string)							{ Write(static_cast<const wchar_t*>(string)); }


	//!Counterpart to WritePlain. Only Read buf(char*).
	/** \note Assert that you dont read about the written/used buffer.*/
	void ReadPlain(volatile void* buf, unsigned _size)
	{
		_ASSERT(roffset + _size <= woffset);
		memcpy(const_cast<void*>(buf), buffer + roffset, _size);	roffset += _size;
	}

	//!Counterpart to Write.
	/**Read _size(unsigned int), alloc string (if wished) and read string(char*).
	\attention Assert that you dont read about the written/used buffer. Dont forget to delete[] the string!*/
	bool Read(char** str, unsigned int tokenlimit = -1, bool allocate_array=true)
	{
		unsigned int _size;
		Read(_size);

		if(_size >= tokenlimit)	return false;
		_ASSERT(roffset + _size <= woffset);

		if(allocate_array) *str = new char[_size];
		_ASSERT(*str);
		memcpy(*str, buffer + roffset, _size);		roffset += _size;
		return true;
	}

	//!Counterpart to Write.
	/**Read _size(unsigned int), alloc string, read string(char*) and convert it from multibyte.
	\attention Assert that you dont read about the written/used buffer. Dont forget to delete[] the string!*/
	bool Read(wchar_t** str, unsigned int tokenlimit = -1, bool allocate_array=true)
	{
		_ASSERT(roffset + sizeof(int) <= woffset);

		unsigned int _size;
		memcpy(&_size, buffer + roffset, sizeof(int));	roffset += sizeof(int);

		if(_size >= tokenlimit)	return false;
		_ASSERT(roffset + _size <= woffset);

		if(allocate_array) *str = new wchar_t[_size];
		_ASSERT(*str);
		mbs2wcs_n(*str, buffer + roffset, _size);		roffset += _size;
		return true;
	}
	//!Counterpart to Write.
	/**Read _size(unsigned int), alloc string, read string, copy it and deallocs string.
	\attention Assert that you dont read about the written/used buffer.*/
	template<typename T> bool Read(std::basic_string<T> &string, unsigned int tokenlimit = -1)
	{
		T* str = 0;
		if(!Read(&str, tokenlimit))	return false;
		string = str;
		delete[] str;
		return true;
	}

	//!redirector (template).
	template<class T1> void Read(T1 &t)						{ ReadPlain(&t, sizeof(t)); }
	//!redirector for char[]
	bool Read(char str[], unsigned int tokenlimit = -1)		{ return Read(&str, tokenlimit, false); }
	//!redirector for wchar_t[]
	bool Read(wchar_t str[], unsigned int tokenlimit = -1)	{ return Read(&str, tokenlimit, false); }

#ifndef DONT_BOTHER_ME
	//!redirector for ptr*. if you know what you do just add a DONT_BOTHER_ME define.
	template<typename T> void Write(T*)					{ _ASSERT(false); }
	//!redirector for ptr* instances. Should never been called because you should never read/write a ptr directly!
	template<typename T> bool Read(T* ptr)				{ _ASSERT(false); return false; }
#endif
};


template<class T1> NetworkBuffer& operator << (NetworkBuffer &c, const T1& t)
{
	c.Write(t);
	return c;
}

template<class T1> NetworkBuffer& operator >> (NetworkBuffer &c, T1& t)
{
	c.Read(t);
	return c;
}

};
#endif
//interface de-serializable einbauen

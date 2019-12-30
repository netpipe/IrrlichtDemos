#ifndef __AZADIARCHIVE_H__
#define __AZADIARCHIVE_H__

#include <string>
#include "zlib/zlib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <map>

//! Compression facilities
class Compress
{
	public:
		Compress(const std::string& filename);
		Compress();
		~Compress();
		void SetComment(const std::string& comment);
		void AddData(void *buffer, const unsigned int &numBytes, const std::string&);
		void UpdateArchiveData();
		int Deflate(unsigned char *, int size, int level);
	private:
		char * Squish(const char *);

		std::fstream archive;
		unsigned int pos;
		unsigned int nofiles;
		unsigned int archivesize;
		std::vector<char> buff;
};

//! Decompression facilities
class Decomp
{
	public:
		Decomp(const std::string& filename);
		Decomp();
		~Decomp();
		char * GetComment();
		bool SetFile(const std::string&);
		inline unsigned long int getFileSize() { return uncompressedSize; }
		void * GetData(unsigned int size);
		int Inflate(unsigned char *, int size);
	private:
		char * UnSquish(char *buffer);

		std::fstream archive;
		unsigned int pos;
		unsigned int filesize;
		unsigned int uncompressedSize;
		unsigned int archivesize;
		std::vector<char> buff;
};

#endif

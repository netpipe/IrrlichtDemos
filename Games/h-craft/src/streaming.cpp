// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "streaming.h"
#include <cassert>
#include <climits>
#include <cstring>

// --------------------------------------------------------
MemArchive::MemArchive(uint64_t startCapacity_)
{
    mSize = 0;
    mCapacity = startCapacity_;
    if ( mCapacity < 1 )
        mCapacity = 1;
    mReadPos = 0;
    mMaxWriteSize = ULONG_MAX;
    mData = new char[mCapacity];
}

MemArchive::MemArchive(const MemArchive &archive_)
: IArchive()
, mSize(0)
, mCapacity(0)
, mReadPos(0)
, mMaxWriteSize(ULONG_MAX)
, mData(NULL)
{
    *this = archive_;
}

MemArchive& MemArchive::operator=(const MemArchive &archive_)
{
	if ( &archive_ != this )
	{
		delete[] mData;
		mData = NULL;

		mSize = archive_.mSize;
		mReadPos = archive_.mReadPos;
		mCapacity = archive_.mCapacity;
		mMaxWriteSize = archive_.mMaxWriteSize;
		mData = new char[mCapacity];
		memcpy(mData, archive_.mData, mCapacity);
	}

    return *this;
}

MemArchive::~MemArchive()
{
    delete[] mData;
}

bool MemArchive::CapacityCheck(uint64_t additionalSizeNeeded_)
{
    if ( mSize + additionalSizeNeeded_ > mMaxWriteSize )
        return false;
    if ( mSize + additionalSizeNeeded_ > mCapacity )
    {
        char * dummy = mData;
        uint64_t newCapacity = mCapacity * 2 + 1;
        if ( mSize + additionalSizeNeeded_ > newCapacity )
            newCapacity = mSize + additionalSizeNeeded_;
        mData = new char[newCapacity];
        memcpy(mData, dummy, mCapacity);
        delete[] dummy;

        mCapacity = newCapacity;
    }

    return true;
}

void MemArchive::Clear()
{
    mSize = 0;
    mReadPos = 0;
}

void MemArchive::Write(bool val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(char val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(unsigned char val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(int16_t val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(uint16_t val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(int32_t val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(uint32_t val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(int64_t val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(uint64_t val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}


void MemArchive::Write(float val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::Write(double val_)
{
    if ( CapacityCheck(sizeof(val_)) )
    {
        memcpy(&(mData[mSize]), &val_, sizeof(val_));
        mSize += sizeof(val_);
    }
}

void MemArchive::WriteString(const char * str_)
{
    if ( !str_ )
    {
        uint64_t strLen = 0;
        if ( CapacityCheck(sizeof(strLen)) )
        {
            memcpy(&(mData[mSize]), &strLen, sizeof(strLen));
            mSize += sizeof(strLen);
        }
    }
    else
    {
        uint64_t strLen = strlen(str_) + 1;
        if ( CapacityCheck(strLen + sizeof(strLen)) )
        {
            memcpy(&(mData[mSize]), &strLen, sizeof(strLen));
            mSize += sizeof(strLen);
            memcpy(&(mData[mSize]), str_, strLen);
            mSize += strLen;
        }
    }
}

void MemArchive::WriteData(uint64_t size_, void * data_)
{
    if ( CapacityCheck(sizeof(size_) + size_) )
    {
        memcpy(&(mData[mSize]), &size_, sizeof(size_));
        mSize += sizeof(size_);
        if ( size_ )
        {
            assert(data_);
            memcpy(&(mData[mSize]), data_, size_);
            mSize += size_;
        }
    }
}

void MemArchive::WriteRaw(uint64_t size_, void * data_)
{
    if ( CapacityCheck(size_) )
    {
        if ( size_ && data_)
        {
            memcpy(&(mData[mSize]), data_, size_);
            mSize += size_;
        }
    }
}

void MemArchive::ReadRaw(uint64_t size_, void ** data_)
{
    if ( !data_ )
    {
        assert(!"zero data_ is given to MemArchive::ReadRaw");
        return;
    }

    if ( !size_ )
    {
        *data_ = 0;
        return;
    }

    if ( mReadPos + size_ > mSize )
    {
        assert(!"trying to read data from archive behind end in MemArchive::ReadRaw");
        return;
    }
    *data_ = new char[size_];
    memcpy(*data_, &(mData[mReadPos]), size_);
    mReadPos += size_;
}

void MemArchive::Read(bool &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(char &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(unsigned char &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(int16_t &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(uint16_t &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(int32_t &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(uint32_t &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(int64_t &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(uint64_t &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(float &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

void MemArchive::Read(double &val_)
{
    if ( mReadPos + sizeof(val_) > mSize )
    {
        assert(!"trying to read from archive behind end");
        return;
    }
    memcpy(&val_, &(mData[mReadPos]), sizeof(val_));
    mReadPos += sizeof(val_);
}

int64_t MemArchive::ReadString(char ** str_)
{
    if ( !str_ )
    {
        assert(!"zeror str_ is given to MemArchive::ReadString");
        return -1;
    }

    int64_t strlen = 0;
    if ( mReadPos + sizeof(strlen) > mSize )
    {
        assert(!"trying to read from archive behind end in MemArchive::ReadString");
        return -1;
    }
    memcpy(&strlen, &(mData[mReadPos]), sizeof(strlen));
    mReadPos += sizeof(strlen);

    if ( !strlen )
    {
        *str_ = 0;
        return 0;
    }

    if ( mReadPos + strlen > mSize )
    {
        assert(!"trying to read string from archive behind end in MemArchive::ReadString");
        return -1;
    }
    *str_ = new char[strlen];   // not strlen+1, because end (0) is also transported
    memcpy(*str_, &(mData[mReadPos]), strlen);
    mReadPos += strlen;

    return strlen;
}

int64_t MemArchive::ReadData(void ** data_)
{
    if ( !data_ )
    {
        assert(!"zero data_ is given to MemArchive::ReadData");
        return -1;
    }
    int64_t size = 0;
    if ( mReadPos + sizeof(size) > mSize )
    {
        assert(!"trying to read from archive behind end in MemArchive::ReadData");
        return -1;
    }
    memcpy(&size, &(mData[mReadPos]), sizeof(size));
    mReadPos += sizeof(size);

    if ( !size )
    {
        *data_ = 0;
        return 0;
    }

    if ( mReadPos + size > mSize )
    {
        assert(!"trying to read data from archive behind end in MemArchive::ReadData");
        return -1;
    }
    *data_ = new char[size];
    memcpy(*data_, &(mData[mReadPos]), size);
    mReadPos += size;

    return size;
}

// --------------------------------------------------------
FileArchive::FileArchive()
: mFile(0)
{
}

FileArchive::FileArchive(const std::string &filename_, const std::string &mode_)
: mFilename(filename_)
{
    Open(mFilename, mode_);
}

FileArchive::~FileArchive()
{
    Close();
}

bool FileArchive::Open(const std::string &filename_, const std::string &mode_)
{
    Close();
    mFile = fopen(filename_.c_str(), mode_.c_str());
    if ( !mFile )
        return false;
    return true;
}

void FileArchive::Close()
{
    if ( mFile )
    {
        fclose(mFile);
    }
}

void FileArchive::Clear()
{
    if ( mFile )
    {
        fclose(mFile);
        mFile = fopen(mFilename.c_str(), "w");
    }
}

uint64_t FileArchive::GetSize() const
{
    if ( !mFile)
        return 0;

    uint64_t oldPos = ftell(mFile);
    fseek(mFile, 0, SEEK_END);
    uint64_t result = ftell(mFile);
    fseek(mFile, oldPos, SEEK_SET);
    return result;
}

uint64_t FileArchive::GetReadPos() const
{
    if ( !mFile )
        return 0;
    return ftell(mFile);
}

void FileArchive::SetReadPos(uint64_t pos_)
{
    if ( mFile )
    {
        fseek(mFile, pos_, SEEK_SET);
    }
}

void FileArchive::Write(bool val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(char val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(unsigned char val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(int16_t val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(uint16_t val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(int32_t val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(uint32_t val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(int64_t val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(uint64_t val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(float val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Write(double val_)
{
    if ( mFile )
    {
        fwrite(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::WriteString(const char * str_)
{
    if ( !mFile )
        return;
    if ( !str_ )
    {
        uint64_t strLen = 0;
        fwrite(&strLen, sizeof(strLen), 1, mFile);
    }
    else
    {
        uint64_t strLen = strlen(str_) + 1;
        fwrite(&strLen, sizeof(strLen), 1, mFile);
        fwrite(str_, strLen, 1, mFile);
    }
}

void FileArchive::WriteData(uint64_t size_, void * data_)
{
    if ( !mFile )
        return;
    fwrite(&size_, sizeof(size_), 1, mFile);
    if ( size_ )
    {
        fwrite(data_, size_, 1, mFile);
    }
}

void FileArchive::WriteRaw(uint64_t size_, void * data_)
{
    if ( mFile && size_ && data_ )
    {
        fwrite(data_, size_, 1, mFile);
    }
}

void FileArchive::ReadRaw(uint64_t size_, void ** data_)
{
    if ( mFile && size_ && data_ )
    {
        *data_ = new char[size_];
        fread(*data_, size_, 1, mFile);
    }
}

void FileArchive::Read(bool &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(char &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(unsigned char &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(int16_t &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(uint16_t &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(int32_t &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(uint32_t &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(int64_t &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(uint64_t &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(float &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

void FileArchive::Read(double &val_)
{
    if ( mFile )
    {
        fread(&val_, sizeof(val_), 1, mFile);
    }
}

// return 0 for no string, 1 for empty string and strlen+1 otherwise
int64_t FileArchive::ReadString(char ** str_)
{
    if ( !mFile )
    {
        return -1;
    }
    if ( !str_ )
    {
        assert(!"zeror str_ is given to FileArchive::ReadString");
        return -1;
    }

    int64_t strLen = 0;
    fread(&strLen, sizeof(strLen), 1, mFile);

    if ( !strLen )
    {
        *str_ = 0;
        return 0;
    }

    *str_ = new char[strLen];   // not strlen+1, because end (0) is also transported
    fread(*str_, strLen, 1, mFile);

    return strLen;
}

// return size of data
int64_t FileArchive::ReadData(void ** data_)
{
    if ( !mFile )
    {
        return -1;
    }
    if ( !data_ )
    {
        assert(!"zero data_ is given to FileArchive::ReadData");
        return -1;
    }
    int64_t size = 0;
    fread(&size, sizeof(size), 1, mFile);

    if ( !size )
    {
        *data_ = 0;
        return 0;
    }

    *data_ = new char[size];
    fread(*data_, size, 1, mFile);

    return size;
}

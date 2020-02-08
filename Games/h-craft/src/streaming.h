// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef STREAMING_H
#define STREAMING_H

// TODO: support big endian
// TODO: wchar_t currently no longer supported. If it's added again take care that wchar_t has different
// sizes on different systems.

#include <string>
#include <stdio.h>
#include <stdint.h>

class IArchive
{
public:
    virtual ~IArchive() {}

    virtual void Clear() = 0;
    virtual uint64_t GetSize() const = 0;
    virtual uint64_t GetReadPos() const = 0;
    virtual void SetReadPos(uint64_t pos_) = 0;

    virtual void Write(bool val_) = 0;
    virtual void Write(char val_) = 0;
    virtual void Write(unsigned char val_) = 0;
    virtual void Write(int16_t val_) = 0;
    virtual void Write(uint16_t val_) = 0;
    virtual void Write(int32_t val_) = 0;
    virtual void Write(uint32_t val_) = 0;
    virtual void Write(int64_t val_) = 0;
    virtual void Write(uint64_t val_) = 0;
    virtual void Write(float val_) = 0;
    virtual void Write(double val_) = 0;
    virtual void WriteString(const char * str_) = 0;
    virtual void WriteData(uint64_t size_, void * data_) = 0;

    virtual void Read(bool &val_) = 0;
    virtual void Read(char &val_) = 0;
    virtual void Read(unsigned char &val_) = 0;
    virtual void Read(int16_t &val_) = 0;
    virtual void Read(uint16_t &val_) = 0;
    virtual void Read(int32_t &val_) = 0;
    virtual void Read(uint32_t &val_) = 0;
    virtual void Read(int64_t &val_) = 0;
    virtual void Read(uint64_t &val_) = 0;
    virtual void Read(float &val_) = 0;
    virtual void Read(double &val_) = 0;
    virtual int64_t ReadString(char ** str_) = 0;   // return 0 for no string, 1 for empty string and strlen+1 otherwise
    virtual int64_t ReadData(void ** data_) = 0;    // return size of data

    // don't append size information but just write the data_
    virtual void WriteRaw(uint64_t size_, void * data_) = 0;
    virtual void ReadRaw(uint64_t size_, void ** data_) = 0;

    // comfort functions
    virtual int16_t  Read16s()  { int16_t  result; Read(result); return result; }
    virtual uint16_t Read16u()  { uint16_t result; Read(result); return result; }
    virtual int32_t  Read32s()  { int32_t  result; Read(result); return result; }
    virtual uint32_t Read32u()  { uint32_t result; Read(result); return result; }
    virtual int64_t  Read64s()  { int64_t  result; Read(result); return result; }
    virtual uint64_t Read64u()  { uint64_t result; Read(result); return result; }
};

class MemArchive : public IArchive
{
public:
    MemArchive(uint64_t startCapacity_ = 1);    // capacity will always be min. 1
    MemArchive(const MemArchive &archive_);
    virtual ~MemArchive();

    MemArchive& operator=(const MemArchive &archive_);

    virtual void Clear();
    virtual uint64_t GetSize() const       { return mSize; }
    virtual uint64_t GetReadPos() const    { return mReadPos; }
    virtual void SetReadPos(uint64_t pos_) { mReadPos = pos_; }

    // Write function will stop working when archive size would get larger than size_.
    // It will _not_ remove data from archives which are already larger.
    virtual void SetMaxWriteSize(uint64_t size_)   { mMaxWriteSize = size_; }

    virtual void Write(bool val_);
    virtual void Write(char val_);
    virtual void Write(unsigned char val_);
    virtual void Write(int16_t val_);
    virtual void Write(uint16_t val_);
    virtual void Write(int32_t val_);
    virtual void Write(uint32_t val_);
    virtual void Write(int64_t val_);
    virtual void Write(uint64_t val_);
    virtual void Write(float val_);
    virtual void Write(double val_);
    virtual void WriteString(const char * str_);
    virtual void WriteData(uint64_t size_, void * data_);

    virtual void Read(bool &val_);
    virtual void Read(char &val_);
    virtual void Read(unsigned char &val_);
    virtual void Read(int16_t &val_);
    virtual void Read(uint16_t &val_);
    virtual void Read(int32_t &val_);
    virtual void Read(uint32_t &val_);
    virtual void Read(int64_t &val_);
    virtual void Read(uint64_t &val_);
    virtual void Read(float &val_);
    virtual void Read(double &val_);
    virtual int64_t ReadString(char ** str_);   // return 0 for no string, 1 for empty string and strlen+1 otherwise
    virtual int64_t ReadData(void ** data_);    // return size of data

    // don't append size information but just write the data_
    virtual void WriteRaw(uint64_t size_, void * data_);
    virtual void ReadRaw(uint64_t size_, void ** data_);

private:
    bool CapacityCheck(uint64_t additionalSizeNeeded_);

    uint64_t mSize;
    uint64_t mCapacity;
    uint64_t mReadPos;
    uint64_t mMaxWriteSize;
    char * mData;
};

class FileArchive : public IArchive
{
public:
    FileArchive();
    FileArchive(const std::string &filename_, const std::string &mode_);
    virtual ~FileArchive();

    // mode_: all fopen flags, typically used here: "wb" writing, "rb" reading
    bool Open(const std::string &filename_, const std::string &mode_);
    void Close();

    virtual void Clear();
    virtual uint64_t GetSize() const;
    virtual uint64_t GetReadPos() const;
    virtual void SetReadPos(uint64_t pos_);

    virtual void Write(bool val_);
    virtual void Write(char val_);
    virtual void Write(unsigned char val_);
    virtual void Write(int16_t val_);
    virtual void Write(uint16_t val_);
    virtual void Write(int32_t val_);
    virtual void Write(uint32_t val_);
    virtual void Write(int64_t val_);
    virtual void Write(uint64_t val_);
    virtual void Write(float val_);
    virtual void Write(double val_);
    virtual void WriteString(const char * str_);
    virtual void WriteData(uint64_t size_, void * data_);

    virtual void Read(bool &val_);
    virtual void Read(char &val_);
    virtual void Read(unsigned char &val_);
    virtual void Read(int16_t &val_);
    virtual void Read(uint16_t &val_);
    virtual void Read(int32_t &val_);
    virtual void Read(uint32_t &val_);
    virtual void Read(int64_t &val_);
    virtual void Read(uint64_t &val_);
    virtual void Read(float &val_);
    virtual void Read(double &val_);
    virtual int64_t ReadString(char ** str_);   // return 0 for no string, 1 for empty string and strlen+1 otherwise
    virtual int64_t ReadData(void ** data_);    // return size of data

    // don't append size information but just write the data_
    virtual void WriteRaw(uint64_t size_, void * data_);
    virtual void ReadRaw(uint64_t size_, void ** data_);

private:
    FILE * mFile;
    std::string mFilename;
};


#endif // STREAMING_H

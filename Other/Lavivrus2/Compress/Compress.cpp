// AzadiArchive.cpp : Defines the entry point for the console application.
//

#include "Compress.h"
#include <string.h>

using namespace std;

Compress::Compress()
{

}

//! Starts the compression of a file
Compress::Compress(const std::string& filename)
{
    char sig[2];
    nofiles = 0;
    archivesize=0;
    archive.open(filename.c_str(), ios_base::out | ios_base::binary);

    sig[1] = sig[0] = 'A';
    archive.write(sig, 2);
    archive.seekp(3);

    archive.write((char*)&nofiles, sizeof(unsigned int));
    archive.write((char*)&archivesize, sizeof(unsigned int));
}

//! Closes the archive, and adds a crc to the end (to do)
Compress::~Compress()
{
    archive.close();
}

//! Sets the comment for the archive
void Compress::SetComment(const std::string& comment)
{
    unsigned short loc;

    archive.seekp(13);
    archive.write(comment.c_str(), unsigned(strlen(comment.c_str())));
    loc = unsigned(archive.tellp());
    pos = loc;
    archive.seekp(11);
    archive.write((char*)&loc, sizeof(unsigned short));
    archive.seekp(pos);
    archivesize = pos;
}

//! Adds data to an archive
void Compress::AddData(void *buffer, const unsigned int &numBytes, const std::string& filename)
{
    unsigned int size;
    char *result = NULL;
    result = Squish((char*)buffer);
    size = unsigned(buff.size());
    archive.write((char*)&size, sizeof(unsigned int));
    archive.write((char*)&numBytes, sizeof(unsigned int));
    archive.put((char)strlen(filename.c_str()));
    archive.write(filename.c_str(), unsigned(strlen(filename.c_str())));
    archive.write(result, size);
    pos = unsigned(archive.tellp());
    nofiles++;
    archivesize = pos;
    UpdateArchiveData();
    buff.clear();
}

//! Updates the archive after each consecutive add of data
void Compress::UpdateArchiveData()
{
    archive.seekp(3);
    archive.write((char*)&nofiles, unsigned(sizeof(unsigned int)));
    archive.write((char*)&archivesize, unsigned(sizeof(unsigned int)));
    archive.seekp(pos);
}

//! Compresses the data given
char * Compress::Squish(const char *data)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[4097];
    unsigned char out[4097];
    char *result;
    unsigned int templength;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, 6);
    if (ret != Z_OK)
    {
        printf("Could not start the deflate engine, data will not be compressed\n");
        return (char*)data;
    }

    templength = 0;

    do
    {
        if (strlen(data) < 4096)
        {
            memcpy(in, data, strlen(data));
            flush = Z_FINISH;
            in[strlen(data)] = '\0';
            data+=templength;
            templength = 0;
        }
        else
        {
            memcpy(in, data, 4096);
            data+=4096;
            templength = 0;
            flush = Z_NO_FLUSH;
            in[4096] = '\0';
        }

        templength = strlen((const char*)in);
        strm.avail_in = unsigned(templength);
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        do
        {
            strm.avail_out = 4096;
            strm.next_out = out;

            ret = deflate(&strm, flush);
            assert(ret != Z_STREAM_ERROR);

            have = 4096 - strm.avail_out;
            for (unsigned int i=0;i < have;i++)
            {
                buff.push_back(out[i]);
            }
        }while (strm.avail_out == 0);
        assert(strm.avail_in == 0);
    }while(flush != Z_FINISH);

    result = new char[buff.size()];

    for (unsigned int x=0; x<buff.size(); x++)
    {
        result[x] = buff[x];
    }

    deflateEnd(&strm);

    return result;
}

int Compress::Deflate(unsigned char *buffer, int size, int level)
{
    unsigned char *tempBuff = new unsigned char[size+14];
    unsigned long length = size+14;
    if (compress2(tempBuff, &length, buffer, size, level) != Z_OK)
        return -1;

    memset(buffer, 0, size);
    memcpy(buffer, tempBuff, length);
    delete [] tempBuff;

    return length;
}

Decomp::Decomp()
{

}

//! Starts the decompression engine for the specified file
Decomp::Decomp(const std::string& filename)
{
    char sig[2];
    unsigned int nofiles;
    unsigned short firstpos;
    char *comment;
    char *p;

    archive.open(filename.c_str(), ios_base::in | ios_base::binary);
    archive.read(sig, 2);
    if (!(sig[0] == sig[1]) && !(sig[0] == 'A'))
    {
        printf("Specified file is not an Azadi Archive\n");
    }

    archive.seekp(3);
    archive.read((char*)&nofiles, sizeof(unsigned int));
    if (nofiles == 0)
    {
        printf("Warning! This archive contains no files\n");
    }
    archive.read((char*)&archivesize, sizeof(unsigned int));
    printf("Archive size: %u\n", archivesize);

    archive.read((char*)&firstpos, sizeof(unsigned short));
    comment = new char[firstpos - 14];
    archive.read(comment, (firstpos - 13));
    p = comment;
    p+= (firstpos-14);
    memset(p, '\0', 1);
    printf("%s\n", comment);
    pos = archive.tellp(); // Save the current position
}

//! Sets which file in the archive to retrieve
bool Decomp::SetFile(const std::string &wantedfile)
{
    char filename[255];
    char filenamesize;
    bool found;
    unsigned int nextjump;

    archive.seekp(pos); // Return to the first entry
    found = false;
    while (!found)
    {
        archive.read((char*)&filesize, sizeof(unsigned int));
        archive.read((char*)&uncompressedSize, sizeof(unsigned int));
        archive.read(&filenamesize, 1);
        archive.read(filename, filenamesize);
        filename[(short)filenamesize] = '\0';
        if (strcmp(filename, wantedfile.c_str()) == 0)
        {
            found = true;
            break;
        }
        if (archive.eof())
            break;
        nextjump = archive.tellp();
        archive.seekp(nextjump + filesize);
    }
    return found;
}

//! Decompresses the data from a buffer
char * Decomp::UnSquish(char *buffer)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[4097];
    unsigned char out[4097];
    unsigned int templength;
    int filelength;

    char *temp;
    char *result;

    temp = new char[strlen(buffer)];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return "";

    templength = 0;

    filelength = filesize;

    do
    {
        if (filelength < 4096)
        {
            memcpy(in, buffer, filelength);
            buffer+=templength;
            templength = 0;
            in[filelength] = '\0';
            flush = Z_FINISH;
            strm.avail_in = filelength;
            filelength=0;
        }
        else
        {
            memcpy(in, buffer, 4096);
            buffer+=templength;
            templength = 0;
            in[4096] = '\0';
            flush = Z_NO_FLUSH;
            filelength -= 4096;
            strm.avail_in = 4096;
        }

        templength = strlen((const char*)in);
        if (strm.avail_in <= 0)
        {
            break;
        }
        strm.next_in = in;

        do
        {
            strm.avail_out = 4096;
            strm.next_out = out;

            ret = inflate(&strm, flush);
            assert(ret != Z_STREAM_ERROR);
            switch (ret)
            {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    printf("Something bad happened\nRet = %d", ret);
                    return "";
            }
            have = 4096 - strm.avail_out;
            for (unsigned int i=0;i<have;i++)
            {
                buff.push_back(out[i]);
            }
        }while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    result = new char[buff.size()+1];

    for (unsigned int i=0; i<buff.size(); i++)
        result[i] = buff[i];

    result[buff.size()] = '\0';

    inflateEnd(&strm);

    buff.clear();

    return result;
}

int Decomp::Inflate(unsigned char *buffer, int size)
{
    unsigned char *newBuff = new unsigned char[size*2];
    unsigned long length;
    if (uncompress(newBuff, &length, buffer, size) != Z_OK)
        return -1;
    memset(buffer, 0, size*2);
    memcpy(buffer, newBuff, length);
    delete [] newBuff;

    return length;
}

//! Returns the decompressed data of a file
void * Decomp::GetData(unsigned int size)
{
    char *buffer;
    if (size == 0)
    {
        buffer = new char[filesize];
        archive.read(buffer, filesize);
        buffer[filesize] ='\0';
    }
    else
    {
        buffer = new char[size];
        archive.read(buffer, size);
        buffer[size] = '\0';
    }
    return UnSquish(buffer);
}

Decomp::~Decomp()
{
    archive.close();
}


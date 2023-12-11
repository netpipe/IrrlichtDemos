#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <cstdio>
#include <string>

#include "../Compress/Compress.h"

class FileSystem
{
    public:
        FileSystem(const std::string &fileName, char *flags);
        ~FileSystem();

        void read(void *buffer, const unsigned int &numBytes, const bool &bigEndian);
        void write(void *buffer, const unsigned int &numBytes, const bool &bigEndian);
        unsigned long int tellg();
        unsigned long int getFileSize();
        void close();

        operator bool() { return available; }

        void *getMmapHandle() { return mmapHandle; }
        FILE *getCFileHandle() { return file; }
    private:
        void *mmapHandle;
        FILE *file;
        bool available;
        bool mmapped;
        unsigned long int mmapPosition;

        Compress *comp;
        Decomp *decomp;
};

#endif

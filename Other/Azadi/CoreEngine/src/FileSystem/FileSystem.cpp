#include "FileSystem.h"
#include <string.h>

FileSystem::FileSystem(const std::string &filename, char *flags)
{
    file = NULL;
    if (filename.find(".aa") != filename.npos)
    {
        std::string fileName = filename.substr(filename.find(".aa") + 5);
        unsigned int flagLen = strlen(flags);
        for (unsigned int i = 0; i < flagLen; ++i)
        {
            if (flags[i] == 'r')
            {
                comp = NULL;
                decomp = new Decomp(filename.substr(0, filename.find(".aa") + 3));
                if (decomp->SetFile(fileName))
                {
                    // allocate enough space for the entire file
                    char *fileData = new char[decomp->getFileSize()];
                    fileData = (char*)decomp->GetData(decomp->getFileSize());
                    mmapHandle = fileData;
                    mmapPosition = 0;
                }
                else
                {
                    available = false;
                    delete decomp;
                }
                break;
            }
            else if (flags[i] == 'w')
            {
                decomp = NULL;
                comp = new Compress(filename.substr(0, filename.find(".aa")));
                // Compression needs work in this area. we probably won't ever need this branch at all.
                break;
            }
        }
    }
    else
    {
        file = fopen(filename.c_str(), flags);
        mmapped = false;
    }
    if (!file)
        available = false;
    else
        available = true;
}

FileSystem::~FileSystem()
{
    if (file != NULL)
        fclose(file);
}

void FileSystem::read(void *buffer, const unsigned int &numBytes, const bool &bigEndian)
{
    if (!mmapped)
    {
        fread(buffer, numBytes, 1, file);
    }
    else
    {
        char *p = (char*)mmapHandle;
        p += mmapPosition;
        for (unsigned int i = 0; i < numBytes; ++i)
        {
            memcpy(buffer, p, numBytes);
        }
        mmapPosition += numBytes;
    }
}

void FileSystem::write(void *buffer, const unsigned int &numBytes, const bool &bigEndian)
{
    if (!mmapped)
    {
        fwrite(buffer, numBytes, 1, file);
    }
    else
    {
        // to do still
    }
}

void FileSystem::close()
{
    fclose(file);
    file = NULL;
}

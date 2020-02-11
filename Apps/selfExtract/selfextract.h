#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

//cat main test.txt test.zip > combined
//./combined apple // string to parse

int getExecutablePath(char* szBuff, unsigned long ulBufferLen)
{
    if (szBuff == NULL || ulBufferLen < 256) {
        return -1;
    }
    char id[256];
    sprintf(id, "/proc/%d/exe", getpid());
    readlink(id, szBuff, ulBufferLen);
    return 0;
}

int mainextract(int argc, char *argv[])
{
    if (argc != 2) {
        puts("Not found splitter argument\n");
        return -1;
    }

    char *szSplitter = argv[1];
    size_t nSplitterLen = strlen(szSplitter);
    char szPath[4096] = {0,};

    if (getExecutablePath(szPath, sizeof(szPath)) < 0) {
        puts("Fail to get the executable file path\n");
    }

    int hExeFile = open(szPath, O_RDONLY);
    if (hExeFile < 0) {
        puts("Fail to open the executable file\n");
        return -1;
    }

    struct stat exeFileStat;
    if (fstat(hExeFile, &exeFileStat)) {
        puts("Unable to get the stat of the executable file\n");
        close(hExeFile);
        return -1;
    }

    long lExeFileSize = exeFileStat.st_size;
    char *pchExeFileMap = (char *)mmap(0, lExeFileSize, PROT_READ, MAP_SHARED, hExeFile, 0);
    if (pchExeFileMap == MAP_FAILED) {
        puts("Fail to create file map\n");
        close(hExeFile);
        return -1;
    }

    unsigned int nIndex = 0;
    long lRealExeFileSize = 0;
    long lPos = -1;
    long lPointer = 0;

    for (lPointer = 0; lPointer < lExeFileSize; ++lPointer) {
        if (pchExeFileMap[lPointer] == szSplitter[nIndex]) {
            ++nIndex;
            if (nIndex == nSplitterLen) {
                lRealExeFileSize = lPointer - nIndex + 1;
                lPos = lPointer + 1;
                break;
            }
        } else {
            nIndex = 0;
        }
    }

    if (lPos < 0) {
        puts("Not found the splitter string\n");
        munmap((void *)pchExeFileMap, lExeFileSize);
        close(hExeFile);
        return -1;
    }

    int hExtractFile = open("extract.zip", O_WRONLY|O_CREAT);
    if (hExtractFile < 0) {
        puts("Unable to create the file extracted\n");
        munmap((void *)pchExeFileMap, lExeFileSize);
        close(hExeFile);
        return -1;
    }

    long lAppendedDataSize = lExeFileSize - lPos;
    printf("Total = %08lX, position = %08lX, size=%08lX\n", lExeFileSize, lPos, lAppendedDataSize);

    if (lAppendedDataSize != write(hExtractFile, pchExeFileMap + lPos, lAppendedDataSize)) {
        puts("An error occured while writing data to the file extracted\n");
        munmap((void *)pchExeFileMap, lExeFileSize);
        close(hExeFile);
        close(hExtractFile);
        return -1;
    }

    close(hExtractFile);

    int hOriginalExeExeFile = creat("original-executable", O_WRONLY|O_CREAT );
    if (hOriginalExeExeFile < 0) {
        puts("Unable to create the executable file stripped append data\n");
        munmap((void *)pchExeFileMap, lExeFileSize);
        close(hExeFile);
        return -1;
    }


    if (lRealExeFileSize != write(hOriginalExeExeFile, pchExeFileMap, lRealExeFileSize) ) {
        puts("Unable to create the executable file stripped append data\n");
        munmap((void *)pchExeFileMap, lExeFileSize);
        close(hExeFile);
        return -1;
    }

    close(hOriginalExeExeFile);
    munmap((void *)pchExeFileMap, lExeFileSize);
    close(hExeFile);
    puts("Successfully extracted\n");
    return 0;
}

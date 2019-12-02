// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CIMGArchiveLoader.h"

#include "CGTAFileList.h"

#include "IReadFile.h"

CIMGArchiveLoader::CIMGArchiveLoader(io::IFileSystem* fs)
{
    FileSystem = fs;
}

CIMGArchiveLoader::~CIMGArchiveLoader()
{
}

bool CIMGArchiveLoader::isALoadableFileFormat(const path &filename) const
{
	return core::hasFileExtension(filename, "img");
}

bool CIMGArchiveLoader::isALoadableFileFormat(io::IReadFile* file) const
{
    // since all IMG files must have extension .img, we can skip this part and return false
    return false;
}

IFileArchive* CIMGArchiveLoader::createArchive(const path &filename, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;
	io::IReadFile* file = FileSystem->createAndOpenFile(filename);

	if (file)
	{
		archive = createArchive(file, ignoreCase, ignorePaths);
		file->drop();
	}

	return archive;
}




void CIMGReader::init()
{
    SIMGFileHeaderV2 header;

    File->read(&header, sizeof(SIMGFileHeaderV2));

    if(header.fourcc[0] == 'V' && header.fourcc[1] == 'E' && header.fourcc[2] == 'R' && header.fourcc[3] == '2')
    {
        // v2 IMG file

        // load directory into memory

        SIMGDirEntry dirEntry;
        SIMGFileEntry fileEntry;

        u32 i;
        for(i=0; i < header.entries; i++)
        {
            File->read(&dirEntry, sizeof(SIMGDirEntry));

            fileEntry.simpleFileName = dirEntry.name;
            fileEntry.pos = dirEntry.pos * 2048;
            fileEntry.length = dirEntry.length * 2048;

            if(IgnoreCase)
            {
                fileEntry.simpleFileName.make_lower();
            }

			FileList->addEntrya(fileEntry.simpleFileName, fileEntry.pos, fileEntry.length);
        }
    }
    else
    {
        IReadFile* DirFile;

        path dirFileName = File->getFileName();
        s32 offset = dirFileName.findLast('.');
        dirFileName[offset+1] = 'd';
        dirFileName[offset+2] = 'i';
        dirFileName[offset+3] = 'r';

        DirFile = FileSystem->createAndOpenFile(dirFileName);

        if (DirFile)
        {
            // v1 IMG file

            // load directory into memory

            SIMGDirEntry dirEntry;
            SIMGFileEntry fileEntry;

            while(DirFile->read(&dirEntry, sizeof(SIMGDirEntry)) == sizeof(SIMGDirEntry))
            {
                fileEntry.simpleFileName = dirEntry.name;
                fileEntry.pos = dirEntry.pos * 2048;
                fileEntry.length = dirEntry.length * 2048;

                if(IgnoreCase)
                {
                    fileEntry.simpleFileName.make_lower();
                }

				FileList->addEntrya(fileEntry.simpleFileName, fileEntry.pos, fileEntry.length);
            }
        }
        else
        {
            // unsupported IMG file, or DIR file not found
        }
    }
}

IReadFile* CIMGReader::createAndOpenFile(const io::path& filename)
{
	const s32 index = FileList->findFile(filename, false);

	if (index != -1)
    {
        return createAndOpenFile(index);
    }

	return 0;
}

IReadFile* CIMGReader::createAndOpenFile(u32 index)
{
    return FileSystem->createLimitReadFile(FileList->getFileName(index), File, FileList->getFilePos(index), FileList->getFileSize(index));
}

const IFileList* CIMGReader::getFileList() const
{
	return FileList;
}

void CIMGReader::setFileSystem(io::IFileSystem* fs)
{
    FileSystem = fs;
}

CIMGReader::CIMGReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
{
    Type = "img";
    File = file;
    IgnoreCase = ignoreCase;
    IgnorePaths = ignorePaths;

	FileList = new CGTAFileList(file->getFileName(), ignoreCase, ignorePaths);

	if(File)
	{
		File->grab();
	}
}
IFileArchive* CIMGArchiveLoader::createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;
	if (file)
	{
		file->seek(0);
		archive = new CIMGReader(file, ignoreCase, ignorePaths);
        ((CIMGReader *)archive)->setFileSystem(FileSystem);
        ((CIMGReader *)archive)->init();
	}
	return archive;
}
CIMGReader::~CIMGReader()
{
	if (File)
    {
        File->drop();
    }
}

 const io::path& CIMGReader::getArchiveName() const
{};

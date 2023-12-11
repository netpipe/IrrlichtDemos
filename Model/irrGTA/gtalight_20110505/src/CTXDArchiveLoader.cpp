// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CTXDArchiveLoader.h"

#include "IReadFile.h"

CTXDArchiveLoader::CTXDArchiveLoader(io::IFileSystem* fs)
{
    FileSystem = fs;
}

CTXDArchiveLoader::~CTXDArchiveLoader()
{
}

bool CTXDArchiveLoader::isALoadableFileFormat(const io::path& filename) const
{
	return core::hasFileExtension(filename, "txd");
}

bool CTXDArchiveLoader::isALoadableFileFormat(io::IReadFile* file) const
{
    // since all TXD files must have extension .TXD, we can skip this part and return false
    return false;
}

IFileArchive* CTXDArchiveLoader::createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const
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

IFileArchive* CTXDArchiveLoader::createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const
{
	IFileArchive *archive = 0;

    if (file)
    {
        file->seek(0);
        archive = new CTXDReader(file, ignoreCase, ignorePaths);
        ((CTXDReader *)archive)->setFileSystem(FileSystem);
    }

	return archive;
}

CTXDReader::CTXDReader(IReadFile* file, bool ignoreCase, bool ignorePaths)
{
    Type = "TXD";
    File = file;
    IgnoreCase = ignoreCase;
    IgnorePaths = ignorePaths;
    TXDName = file->getFileName();
    core::deletePathFromFilename(TXDName);
    TXDName = TXDName.subString(0, TXDName.size() - 4);

	FileList = new CGTAFileList(file->getFileName(), ignoreCase, ignorePaths);

	if(File)
	{
		File->grab();

        parseRWSection(File->getSize(), 0, 0);
	}
}

CTXDReader::~CTXDReader()
{
	if (File)
    {
        File->drop();
    }
}

IReadFile* CTXDReader::createAndOpenFile(const io::path& filename)
{
	s32 index = FileList->findFile(filename, false);

	if (index != -1)
    {
        return createAndOpenFile(index);
    }

	return 0;
}

IReadFile* CTXDReader::createAndOpenFile(u32 index)
{
    return FileSystem->createLimitReadFile(FileList->getFileName(index), File, FileList->getFilePos(index), FileList->getFileSize(index));
}

void CTXDReader::parseRWSection(int size, int depthlevel, int parentType)
{
    if(size == 0) {return;}

    unsigned int sectionType, sectionSize;
    unsigned short sectionunknown = 0, sectionVersion = 0;

    int endoffset = File->getPos() + size;

    while(File->getPos() < endoffset)
    {
        File->read(&sectionType, sizeof(int));
        File->read(&sectionSize, sizeof(int));
        File->read(&sectionunknown, sizeof(short));
        File->read(&sectionVersion, sizeof(short));

        switch(sectionType)
        {
            case DFF_RW_DATA:
                processRWData(sectionSize, parentType, sectionVersion);
                break;

            // all below can contain more sections
            case DFF_RW_EXTENSION:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_DICTIONARY:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            case DFF_RW_TEXTURE_NATIVE:
                parseRWSection(sectionSize, depthlevel + 1, sectionType);
                break;

            default:
                // unknown section type, skip sectionSize bytes
                File->seek(sectionSize, true);
                break;
        }
    }
}

void CTXDReader::processRWData(int size, int type, int version)
{
    if(type == DFF_RW_TEXTURE_NATIVE)
    {
        long pos = File->getPos();

        STXDTextureHeader header;

        File->read(&header, sizeof(STXDTextureHeader));

        File->seek(size - sizeof(STXDTextureHeader), true);

        STXDFileEntry fileEntry;

        fileEntry.simpleFileName = TXDName;
        fileEntry.simpleFileName += "/";
        fileEntry.simpleFileName += header.diffuseTextureName;
        fileEntry.simpleFileName += ".gtatexture";
        fileEntry.pos = pos;
        fileEntry.length = size;

        if(IgnoreCase)
        {
            fileEntry.simpleFileName.make_lower();
        }

		FileList->addEntrya(fileEntry.simpleFileName, fileEntry.pos, fileEntry.length);

        // repeat for alphaTextureName

        fileEntry.simpleFileName = TXDName;
        fileEntry.simpleFileName += "/";
        fileEntry.simpleFileName += header.alphaTextureName;
        fileEntry.simpleFileName += ".gtatexture";

        if(IgnoreCase)
        {
            fileEntry.simpleFileName.make_lower();
        }

		FileList->addEntrya(fileEntry.simpleFileName, fileEntry.pos, fileEntry.length);
    }
    else
    {
        // unknown data section type, skip size bytes
        File->seek(size, true);
    }
}

const IFileList* CTXDReader::getFileList() const
{
	return FileList;
}

void CTXDReader::setFileSystem(io::IFileSystem* fs)
{
    FileSystem = fs;
}


const io::path& CTXDReader::getArchiveName() const{};

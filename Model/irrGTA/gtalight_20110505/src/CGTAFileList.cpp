// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CGTAFileList.h"

CGTAFileList::CGTAFileList(const io::path& path, bool ignoreCase, bool ignorePaths)
{
	IgnoreCase = ignoreCase;
	IgnorePaths = ignorePaths;
	Path = path;
}

CGTAFileList::~CGTAFileList()
{
}

u32 CGTAFileList::getFileCount() const
{
	return FileList.size();
}

const path& CGTAFileList::getFileName(u32 index) const
{
	return FileList[index].simpleFileName;
}

const path& CGTAFileList::getFullFileName(u32 index) const
{
	return FileList[index].simpleFileName;
}

u32 CGTAFileList::getFileSize(u32 index) const
{
	return FileList[index].length;
}

u32 CGTAFileList::getFilePos(u32 index) const
{
	return FileList[index].pos;
}

u32 CGTAFileList::getID(u32 index) const
{
	return index;
}

bool CGTAFileList::isDirectory(u32) const
{
	return false;
}

s32 CGTAFileList::findFile(const path& filename, bool isFolder=false) const
{
    path simpleFileName = filename;

	if (IgnoreCase)
		simpleFileName.make_lower();

	if (IgnorePaths)
		core::deletePathFromFilename(simpleFileName);

    s32 res = -1;
    u32 i;

    // this way of comparison is slow, but binary_search seems buggy
    for(i=0; i < FileList.size(); i++)
    {
        if(FileList[i].simpleFileName == simpleFileName)
        {
            res = i;
            break;
        }
    }

	return res;
}

const path& CGTAFileList::getPath() const
{
	return Path;
}

u32 CGTAFileList::addItem(const path&, u32, bool, u32)
{
	return 0;
}

void CGTAFileList::sort()
{
}

void CGTAFileList::addEntrya(const path& simpleFileName, u32 pos, u32 length)
{
	SFileEntry fileEntry;

	fileEntry.simpleFileName = simpleFileName;
	fileEntry.pos = pos;
	fileEntry.length = length;

	FileList.push_back(fileEntry);
}



u32 CGTAFileList::addItem(const io::path& fullPath,
	u32 offset, u32 size, bool isDirectory, u32 id){};

	u32 CGTAFileList::getFileOffset(u32 index) const{};

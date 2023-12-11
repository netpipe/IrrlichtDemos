// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CIMGARCHIVELOADER_H_
#define _CIMGARCHIVELOADER_H_

#include "CGTAFileList.h"

#include "irrlicht.h"
#include "IReferenceCounted.h"
#include "IReadFile.h"
#include "irrArray.h"
#include "irrString.h"
#include "IFileSystem.h"

using namespace irr;
using namespace core;
using namespace io;

struct SIMGFileHeaderV2
{
    c8 fourcc[4];
    u32 entries;
};

struct SIMGDirEntry
{
    u32 pos;
    u32 length;
    c8 name[24];
};

struct SIMGFileEntry
{
    path simpleFileName;
    u32 pos;
    u32 length;
};

class CIMGArchiveLoader : public IArchiveLoader
{
	public:
		CIMGArchiveLoader(io::IFileSystem* fs);
		virtual ~CIMGArchiveLoader();
		virtual bool isALoadableFileFormat(const io::path& filename) const;
		virtual bool isALoadableFileFormat(io::IReadFile* file) const;
		virtual IFileArchive* createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const;
		virtual io::IFileArchive* createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const;
		virtual bool isALoadableFileFormat(irr::io::E_FILE_ARCHIVE_TYPE) const {return false;}

	private:
		io::IFileSystem* FileSystem;
};


class CIMGReader : public IFileArchive
{
	public:
		CIMGReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~CIMGReader();
		virtual IReadFile* createAndOpenFile(const io::path& filename);
		virtual IReadFile* createAndOpenFile(u32 index);
		virtual const IFileList* getFileList() const;
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_UNKNOWN; }
		void setFileSystem(io::IFileSystem* fs);
		void init();

	virtual const io::path& getArchiveName() const;

	private:
		path Type;
		IReadFile* File;
		CGTAFileList* FileList;
		bool IgnoreCase;
		bool IgnorePaths;
		io::IFileSystem* FileSystem;
};

#endif /* _CIMGARCHIVELOADER_H_ */

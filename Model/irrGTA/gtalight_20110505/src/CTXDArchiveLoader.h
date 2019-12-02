// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CTXDARCHIVELOADER_H_
#define _CTXDARCHIVELOADER_H_

#include "CGTAFileList.h"

#include "irrlicht.h"
#include "IReferenceCounted.h"
#include "IReadFile.h"
#include "irrArray.h"
#include "irrString.h"
#include "IFileSystem.h"

// DFF section IDs
#define DFF_RW_DATA 1
#define DFF_RW_EXTENSION 3
#define DFF_RW_TEXTURE_NATIVE 21
#define DFF_RW_DICTIONARY 22

using namespace irr;
using namespace core;
using namespace io;

struct STXDTextureHeader
{
    u8 ignore[8];
    u8 diffuseTextureName[32];
    u8 alphaTextureName[32];
};

struct STXDFileEntry
{
    path simpleFileName;
    u32 pos;
    u32 length;
};

class CTXDArchiveLoader : public IArchiveLoader
{
	public:

		CTXDArchiveLoader(io::IFileSystem* fs);
		virtual ~CTXDArchiveLoader();
		virtual bool isALoadableFileFormat(const io::path& filename) const;
		virtual bool isALoadableFileFormat(io::IReadFile* file) const;
		virtual IFileArchive* createArchive(const io::path& filename, bool ignoreCase, bool ignorePaths) const;
		virtual io::IFileArchive* createArchive(io::IReadFile* file, bool ignoreCase, bool ignorePaths) const;
		virtual bool isALoadableFileFormat(irr::io::E_FILE_ARCHIVE_TYPE) const {return false;}

	private:
		io::IFileSystem* FileSystem;
		core::array< core::string<path> >* LoadedList;
};


class CTXDReader : public IFileArchive
{
	public:
		CTXDReader(IReadFile* file, bool ignoreCase, bool ignorePaths);
		virtual ~CTXDReader();
		virtual IReadFile* createAndOpenFile(const io::path& filename);
		virtual IReadFile* createAndOpenFile(u32 index);
		virtual const IFileList* getFileList() const;
		virtual E_FILE_ARCHIVE_TYPE getType() const { return EFAT_UNKNOWN; }
		void setFileSystem(io::IFileSystem* fs);
virtual const io::path& getArchiveName() const;
	private:
		path Type;
		path TXDName;
		IReadFile* File;
		CGTAFileList* FileList;
		bool IgnoreCase;
		bool IgnorePaths;
		io::IFileSystem* FileSystem;

		void parseRWSection(int size, int depthlevel, int parentType);
		void processRWData(int size, int type, int version);
};

#endif /* _CTXDARCHIVELOADER_H_ */

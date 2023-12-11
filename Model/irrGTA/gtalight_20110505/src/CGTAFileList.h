// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CGTAFILELIST_H_
#define _CGTAFILELIST_H_

#include "irrlicht.h"
#include "IReferenceCounted.h"
#include "IReadFile.h"
#include "irrArray.h"
#include "irrString.h"
#include "IFileSystem.h"

using namespace irr;
using namespace core;
using namespace io;

struct SFileEntry
{
    path simpleFileName;
    u32 pos;
    u32 length;
};

class CGTAFileList : public IFileList
{
	public:
		CGTAFileList(const io::path&, bool, bool);
		~CGTAFileList();
		virtual u32 getFileCount() const;
		virtual const path& getFileName(u32) const;
		virtual const path& getFullFileName(u32) const;
		virtual u32 getFileSize(u32) const;
		virtual u32 getFilePos(u32) const;
		virtual u32 getID(u32) const;
		virtual bool isDirectory(u32) const;
		virtual s32 findFile(const path&, bool) const;
		virtual const path& getPath() const;

		virtual u32 addItem(const path&, u32, bool, u32);
		virtual void sort();
		void addEntrya(const path&, u32, u32);

	virtual u32 getFileOffset(u32 index) const;
	virtual u32 addItem(const io::path& fullPath,
	u32 offset, u32 size, bool isDirectory, u32 id);

	private:
		core::array<SFileEntry> FileList;
		path Path;
		bool IgnoreCase;
		bool IgnorePaths;
};

#endif /* _CGTAFILELIST_H_ */

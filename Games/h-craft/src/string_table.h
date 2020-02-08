// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef STRING_TABLE_H
#define STRING_TABLE_H

#include <irrlicht.h>
#include <vector>

struct StringTableEntry
{
    StringTableEntry()  {}
    StringTableEntry(const irr::core::stringw &identifier_) : mIdentifier(identifier_) {}
    StringTableEntry(const irr::core::stringw &identifier_, const irr::core::stringw &string_) :  mIdentifier(identifier_), mString(string_) {}

	friend inline bool operator<(const irr::core::stringw& identifier_, const StringTableEntry& rhs);

    bool operator <(const StringTableEntry& other_) const
    {
        return mIdentifier < other_.mIdentifier;
    }

    irr::core::stringw mIdentifier;
    irr::core::stringw mString;
};

inline bool operator<(const irr::core::stringw& identifier_, const StringTableEntry& rhs)
{
	return identifier_ < rhs.mIdentifier;
}

class StringTable
{
public:
	StringTable() : mIrrFileSystem(0) {}

	// set the Irrlicht filesystem (needed on Android)
	void SetIrrFs(irr::io::IFileSystem * fs)
	{
		mIrrFileSystem = fs;
	}

    bool Load(const char* filename_, bool addToExisting=true);

    // return filename used in last Load command
    const char* GetFileName() const;

    // add another string to the string table
    void Add(const irr::core::stringw &identifier_, const irr::core::stringw &string_);

    // Remove all entries
    void Clear();

    // Get the string for identifier_
    // If identifier_ ain't available return the identifier itself.
    irr::core::stringw Get(const irr::core::stringw &identifier_) const;
    irr::core::stringw Get(const char *identifier_) const;

private:
	irr::core::stringc mFileName;
    std::vector<StringTableEntry>  mStringTable;
    irr::io::IFileSystem * mIrrFileSystem;
};


#endif // STRING_TABLE_H

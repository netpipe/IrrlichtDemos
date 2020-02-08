// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HELPER_FILE_H
#define HELPER_FILE_H

#include <string>

namespace ExtFile
{
	// "path/file.abc" will result in "file.abc"
	extern void ErasePathFromFilename(std::string &filename_);
	extern void ErasePathFromFilename(std::wstring &filename_);

	// "path/file.abc" will result in "path/"
	extern void EraseFilenameFromPath(std::string &filename_);
	extern void EraseFilenameFromPath(std::wstring &filename_);

	// "path/file.abc" will result in "path/file".
	// "filename.abc" will result in "filename"
	extern void EraseFiletypeFromFilename(std::string &filename_);
	extern void EraseFiletypeFromFilename(std::wstring &filename_);

	// try to find the real path for the executable
	// both arguments need to be in path form already
	extern std::string FindExePath(const std::string &cwdPath_, const std::string &argv0Path_);

	// exchange all '\' and '\\' by '/'
	extern void MakeLinuxTypePath(std::string &path_);

	// Check if a character can be used in all filesystems which we support
	// If the char is OK, just return it. Otherwise return another char for it,
	// which is ('a'+(check_%26)).
	// allowPathChars_: are those valid: '\' and '/' and ':'
	extern char GetValidFilenameChar(char check_, bool allowPathChars_=false);
};

#endif // HELPER_FILE_H

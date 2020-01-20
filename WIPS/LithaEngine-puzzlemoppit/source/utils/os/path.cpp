
#include "utils/os/path.h"

#if defined(_IRR_WINDOWS_API_)
#include <windows.h>
#include <direct.h> // for mkdir
#else
#include <unistd.h> // geteuid etc
#endif

#include <sys/stat.h> // mkdir, stat

namespace utils
{
namespace os
{
namespace path
{

io::path sepstr()
{
	io::path str;
	str += sep;
	return str;
}

bool exists(const io::path &path)
{
#if defined(_IRR_WINDOWS_API_)
	struct _stat status;
	return _stat(path.c_str(), &status) == 0;
#else
	struct stat status;
	return stat(path.c_str(), &status) == 0;
#endif
}

bool is_dir(const io::path &path)
{
#if defined(_IRR_WINDOWS_API_)
	struct _stat status;
	
	if (_stat(path.c_str(), &status) == -1)
		return false;
	
	return status.st_mode & _S_IFDIR;
#else
	struct stat status;
	
	if (stat(path.c_str(), &status) == -1)
		return false;
	
	return status.st_mode & S_IFDIR;
#endif
}

bool is_file(const io::path &path)
{
#if defined(_IRR_WINDOWS_API_)
	struct _stat status;
	
	if (_stat(path.c_str(), &status) == -1)
		return false;
	
	return status.st_mode & _S_IFREG;
#else
	struct stat status;
	
	if (stat(path.c_str(), &status) == -1)
		return false;
	
	return status.st_mode & S_IFREG;
#endif
}

bool is_executable(const io::path &path)
{
	if (!is_file(path))
		return false;
	
#if defined(_IRR_WINDOWS_API_)
	// For windows we just check the extension.
	// Comparing FindExecutable to the original path would perhaps be a better way.
	io::path ext = getext(path);
	
	if (ext == "exe" || ext == "bat" || ext == "scr" || ext == "com")
		return true;
	else
		return false;
#else
	// For Linux, we check if we have execute permission.
	
	return access(path.c_str(), X_OK) == 0;
	
	// BELOW CODE IS WRONG!! Results don't reflect reality...
	// I tried originally to use stat because of this quote for access(): "an implementation may indicate
	// success for X_OK even if none of the execute file permission bits are set"
	// However I gave up; I can't find any documents describing the exact order of precedence for
	// the execute permissions.
	/*
	struct stat status;
	
	if (stat(path.c_str(), &status) == -1)
		return false;
	
	// If we are root and the file is executable by either user, group, or other.
	if (geteuid() == 0)
		return status.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH);
	
	// Now we check if the current user or group is denied from executing this.
	// Deny takes precedence over allow, so we check if either are denied first.
	
	// If the current user is the file owner, and the file is not user executable.
	if (status.st_uid == geteuid() && !(status.st_mode & S_IXUSR))
		return false;
	
	// If the current user's group is the file's group, and the file is not group executable.
	if (status.st_gid == getegid() && !(status.st_mode & S_IXGRP))
		return false;
	
	// Now we check if the current user or group is allowed to execute this.
	
	if (status.st_uid == geteuid() && status.st_mode & S_IXUSR)
		return true;
	
	if (status.st_gid == getegid() && status.st_mode & S_IXGRP)
		return true;
	
	// We are neither root, the owner, or in the group.
	// Is the file executable by all?
	return status.st_mode & S_IXOTH;
	*/
#endif
}

bool ensure_dir(const io::path &path)
{
	if (is_dir(path))
		return true;
	
#if defined(_IRR_WINDOWS_API_)
	return mkdir(path.c_str()) == 0;
#else
	return mkdir(path.c_str(), S_IRWXU) == 0;
#endif
}

bool ensure_delete(const io::path &path)
{
	if (!exists(path))
		return true;
	
	return remove(path.c_str()) == 0;
}

io::path concat(const io::path &rootPath, const io::path &subDir)
{
	if (rootPath.size() == 0)
		return subDir;
	
	return rtrim(rootPath) + sepstr() + subDir;
}

io::path rtrim(io::path path)
{
	if ( !is_sep(path.lastChar()) )
		return path;
	
	if (path.size() == 0)
		return path;
	
	// Go from right to left, removing separators.
	// If we're not on windows, we exclude checking the initial character.
	// (might have a unix path of "/" that should be preserved)
	
	u32 i = path.size();
	
#if defined(_IRR_WINDOWS_API_)
	const u32 start = 0;
#else
	const u32 start = 1;
#endif
	
	while ( i > start && is_sep(path[i-1]) )
		i --;
	
	if (i == 0)
		return io::path();
	
	return path.subString(0, i);
}

bool is_sep(fschar_t c)
{
	return c == '/' || c == '\\';
}

bool contains_sep(const io::path &path)
{
	for (u32 i = 0; i < path.size(); i ++)
	{
		if (is_sep(path[i]))
			return true;
	}
	return false;
}

io::path basename(const io::path &path)
{
	u32 i = path.size();
	
	while ( i > 0 && !is_sep(path[i-1]) )
		i --;
	
	if (i == 0) // No separator found.
		return path;
	
	return path.subString(i,path.size()-i);
}

io::path dirname(const io::path &path)
{
	u32 i = path.size();
	
	while ( i > 0 && !is_sep(path[i-1]) )
		i --;
	
	if (i == 0) // No separator found.
		return io::path();
	
	return rtrim(path.subString(0,i));
}

bool has_ext(const io::path &path)
{
	return getext(path).size() > 0;
}

io::path getext(const io::path &path)
{
	if (is_sep(path.lastChar())) // it's a directory
		return "";
	
	if (path.lastChar() == '.')
		return "";
	
	if (!str::contains_char(path, '.'))
		return "";
	
	std::vector<core::stringc> parts = str::explode(".", path);
	
	if (parts.size() > 1 && !contains_sep(parts.back()))
		return parts.back();
	else
		return "";
}

std::vector<io::path> splitext(const io::path &path)
{
	std::vector<io::path> result;
	result.push_back("");
	result.push_back("");
	ASSERT(result.size() == 2);
	
	// no extension?
	if (getext(path).size() == 0)
	{
		result[0] = path;
		return result;
	}
	
	std::vector<core::stringc> parts = str::explode(".", path);
	
	// This should have been verified by getext above.
	ASSERT(parts.size() > 1);
	
	for (u32 i = 0; i < parts.size()-1; i ++)
	{
		result[0] += parts[i];
		
		if (i < parts.size()-2)
			result[0] += ".";
	}
	
	result[1] = parts.back();
	
	return result;
}

io::path correctsep(io::path path)
{
	for (u32 i = 0; i < path.size(); i ++)
	{
		if (is_sep(path[i]))
			path[i] = sep;
	}
	
	return path;
}

u32 size(const io::path &path)
{
	FILE *fp = fopen(path.c_str(), "rb");
	
	u32 retval = 0;
	
	if (fp)
	{
		if (fseek(fp, 0, SEEK_END) == 0)
		{
			long int pos = ftell(fp);
			
			if (pos != -1)
				retval = pos;
		}
		
		fclose(fp);
	}
	
	return retval;
}

}
}
}

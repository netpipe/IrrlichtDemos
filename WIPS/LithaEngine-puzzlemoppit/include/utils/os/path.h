
#ifndef UTILS_OS_PATH_H
#define UTILS_OS_PATH_H

#include "litha_internal.h"

namespace utils
{
namespace os
{
namespace path
{

#if defined(_IRR_WINDOWS_API_)
const fschar_t sep = '\\';
#else
const fschar_t sep = '/';
#endif

// return the OS path separator as a string.
io::path sepstr();

// Does the specified file OR directory exist?
bool exists(const io::path &path);

// Is the specified path a valid existing directory.
bool is_dir(const io::path &path);

// Is the specified path a *file* and not a directory.
bool is_file(const io::path &path);

// Is the specified path a file that is executable.
bool is_executable(const io::path &path);

// Ensure a directory exists.
// returns true if success or the directory already exists.
// returns false if the directory cannot be created.
// In Unix this is created with only read/write/execute permissions set for the current user.
bool ensure_dir(const io::path &path);

// Delete a file or directory.
// Returns true if the file is successfully deleted OR if it didn't already exist.
// Returns false if the file does exist but could not be deleted.
bool ensure_delete(const io::path &path);

// Add a subdirectory or file to a path string using the correct OS separator.
// subDir should be a single directory or file name and not contain any separator characters.
io::path concat(const io::path &rootPath, const io::path &subDir);

// Remove any trailing slashes from a path
io::path rtrim(io::path path);

// Is the character a separator? (of any OS type)
bool is_sep(fschar_t c);

// Does the path contain a separator?
bool contains_sep(const io::path &path);

// base name of path. (everything after the final separator)
// This might be either a file or a directory, or an empty string
// if there is nothing after the final separator.
// e.g. /woo/yay returns yay, /woo/yay/ returns nothing.
io::path basename(const io::path &path);

// Returns everything up to (but excluding) the last separator.
// If there are no separators, returns nothing.
// e.g. /woo/yay will return /woo
io::path dirname(const io::path &path);

// Does the path end in a file extension?
bool has_ext(const io::path &path);

// Get the file extension.
// Everything after the last "." (excluding the dot itself)
// Returns an empty string if no file extension.
io::path getext(const io::path &path);

// Split path into all up to the file extension, and the extension itself.
// extension does not contain the .
// Resulting vector always has two elements.
// this is not fully tested.
std::vector<io::path> splitext(const io::path &path);

// Replace any separators in the given path with the correct ones for the OS.
io::path correctsep(io::path path);

// Get size in bytes of a file.
// Will return 0 in case of error (e.g. file not accessible or does not exist)
u32 size(const io::path &path);

}
}
}

#endif


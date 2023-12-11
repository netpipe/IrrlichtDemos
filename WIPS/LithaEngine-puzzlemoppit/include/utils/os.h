
#ifndef UTILS_OS_H
#define UTILS_OS_H

#include "litha_internal.h"

namespace utils
{
namespace os
{

// Get the newline characters for this OS.
// (returns \r\n on Windows, \n on everything else)
core::stringc newline();

// get current working directory
io::path getcwd();

// Change working directory
// Returns false on failure.
bool chdir(const io::path &newPath);

// Return the current user's local application data directory.
// For windows, this may something like C:/Documents and Settings/User/Application Data/
// For unix, this may be just the home directory, e.g. /home/user/
// WARNING: This not yet implemented for Unicode. ASCII only.
io::path getappdata();

// Same as getappdata, but returns a sub directory of it of the specified name.
// Will create the sub directory if it does not already exist.
// Under Linux, this will have a "." prepended so it is hidden.
// If getting the application data or home directory fails for any reason, the sub directory
// wll be created in the working directory.
io::path getcustomappdata(const io::path &dirName);

// List everything in a given directory.
// This includes both files and sub directories, but excludes ".." and "." records.
std::vector<io::path> listdir(const io::path &path);

// List all files in a directory.
// Does not include folders!
// Results are relative not absolute.
std::vector<io::path> listfiles(const io::path &path);

// List all subdirectories in a given directory.
// Files are excluded, as are . and .. records.
std::vector<io::path> listsubdirs(const io::path &path);

// Get the full path of the app's executable.
// Useful for re-launching the app.
io::path getexecutablepath();

// Returns a series of backtrace lines for debugging purposes.
// Exact contents are OS particular.
//std::vector<core::stringc> backtrace();

// Run some external program.
// appPath should be just the executable file and must *NOT* contain any arguments. Unlike normal
// Windows behaviour, you *MUST* give the file extension (usually .exe) along with the executable.
// Arguments are passed as the optional second parameter.
// Returns true on success, false on failure.
// Note that even if true is returned, it is not 100% gauranteed that the app will be run
// due to the myriad of other things that can go wrong down the line.
// secondsBeforeStart - seconds delay before the new program starts running.
// delay occurs in the new program's process, and does NOT hold up the current process.
// Currently only implemented in Unix, ignored in Windows. (starts without delay)
bool run(const io::path &appPath, const std::vector<io::path> &args = std::vector<io::path>(), u32 secondsBeforeStart = 0);

// Attempt to find the absolute path of the given file using the OS's paths.
// The given file should not include any directories.
// Unlike normal Linux behaviour, this will search the current directory first of all.
// Returns an empty string if not found.
io::path searchpath(const io::path &appName);

// Open a web page in the user's default browser.
// This is not 100% guaranteed to succeed. (especially, it may fail on Linux)
void openwebpage(const io::path &url);

// Open a web page if one has not already been opened.
// (so if this is called multiple times, only the first will succeed, subsequent calls will be ignored)
void maybeopenwebpage(const io::path &url);

}
}

#endif

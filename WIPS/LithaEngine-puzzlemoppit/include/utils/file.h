
#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include "litha_internal.h"
#include <vector>
#include "VariantMap.h"

// High level manipulation of file _contents_.
// For file system manipulation, see os::path

namespace utils
{
namespace file
{

// Get the contents of an ASCII text file as a string.
// Returns an empty string on failure.
core::stringc get(const io::path &fileName);

// Put the given ASCII string in the specified file. Overwrites any existing contents.
// Returns false on failure or partial failure.
bool put(const io::path &fileName, const core::stringc &str);

// Get the lines of a text file into a vector.
// Returns an empty vector on failure.
std::vector<core::stringc> get_lines(const io::path &fileName);

// Put the given vector of strings into the specified file, each as a new line, using the current
// OS's newline sequence.
// Overwrites any existing contents.
// Returns false on failure or partial failure.
bool put_lines(const io::path &fileName, const std::vector<core::stringc> &lines);

// Save a map of variants to a Windows .ini style ASCII text file.
// (i.e. a list of lines of the form "key=value")
// Returns false on failure.
bool savesettings(const io::path &fileName, const VariantMap &settings);

// Load a map of variants from a Windows .ini style ASCII text file.
// Returns an empty map on failure.
VariantMap loadsettings(const io::path &fileName);

}
}

#endif


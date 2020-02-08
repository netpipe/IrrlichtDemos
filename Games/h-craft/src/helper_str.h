// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HELPER_STR_H
#define HELPER_STR_H

#include <string>

namespace ExtStr
{
	// "this is a strange function" will be "thisIsAStrangeFunction"
	// or when firstCharUpper_ is set: "ThisIsAStrangeFunction"
	extern void EraseSpacesAndSetUpperCharBehind(std::string &str_, bool firstCharUpper_=false);

	// integer to std::wstring in a platform independent way (there were some problems with wstreams on MinGW)
	extern std::wstring int_to_wstr(int val_);

	// convert between utf8 and widestrings.
	extern std::wstring FromUtf8(const std::string& utf8string_);
	extern std::string ToUtf8(const std::wstring& widestring_);

	// convert string to upper-case characters
	extern void ToUpper(std::string &string_);

	// convert string to lower-case characters
	extern void ToLower(std::string &string_);
};

#endif // HELPER_STR_H

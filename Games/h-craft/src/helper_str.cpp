// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "helper_str.h"
#include "convert_utf.h"

#include <iostream>
#include <sstream>
#include <cctype>

namespace ExtStr
{

void EraseSpacesAndSetUpperCharBehind(std::string &str_, bool firstCharUpper_)
{
    std::string::size_type st = str_.find(" ");
    while ( st != std::string::npos )
    {
        str_.erase(st, 1);
        if ( st < str_.length() && str_[st] != ' ' )
        {
            str_[st] = std::toupper(str_[st]);
        }
        st = str_.find(" ");
    }
    if ( firstCharUpper_ && str_.length() )
    {
        str_[0] = std::toupper(str_[0]);
    }
}


std::wstring int_to_wstr(int val_)
{
    std::ostringstream stream;
    stream << val_;
    std::string str( stream.str() );
    return std::wstring( str.begin(), str.end() );
}

// from http://www.codeproject.com/useritems/UtfConverter.asp
std::wstring FromUtf8(const std::string& utf8string)
{
    size_t widesize = utf8string.length();
    const UTF8* sourcestart = reinterpret_cast<const UTF8*>(utf8string.c_str());
    const UTF8* sourceend = sourcestart + widesize;
    std::wstring result;
    result.reserve(widesize+1); // TODO: guess we can avoid the last cast below when we reserve after conversion. (can't risk changing this now as app does not work for other reasons)

    if (sizeof(wchar_t) == 2)
    {
        UTF16* convstring = new UTF16[widesize+1];
        UTF16* targetstart = &convstring[0];
        UTF16* origstart = targetstart;
        UTF16* targetend = targetstart + widesize;
        ConversionResult res = ConvertUTF8toUTF16(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
        if (res != conversionOK)
        {
            delete[] convstring;
            return L"";
        }
        *targetstart = 0; // points to end now

        for (size_t i=0; origstart <= targetstart; ++origstart, ++i )
            result[i] = (wchar_t)*origstart;
        delete[] convstring;
    }
    else if (sizeof(wchar_t) == 4)
    {
        UTF32* convstring = new UTF32[widesize+1];
        UTF32* targetstart = &convstring[0];
        UTF32* origstart = targetstart;
        UTF32* targetend = targetstart + widesize;
        ConversionResult res = ConvertUTF8toUTF32(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
        if (res != conversionOK)
        {
            delete[] convstring;
            return L"";
        }
        *targetstart = 0;
        for (size_t i=0; origstart <= targetstart; ++origstart, ++i )
            result[i] = (wchar_t)*origstart;
        delete[] convstring;
    }
    return std::wstring( result.c_str() ); // intentional, result can be different from the returned string (probably because of reserved size?)
}

std::string ToUtf8(const wchar_t* widestring_)
{
	if ( !widestring_ || !wcslen(widestring_) )
	{
		std::string str;
		return str;
	}

	return ToUtf8( std::wstring(widestring_) );
}

// from http://www.codeproject.com/useritems/UtfConverter.asp
std::string ToUtf8(const std::wstring& widestring)
{
    size_t widesize = widestring.length();
    std::string resultstring;

    if (sizeof(wchar_t) == 2)
    {
        size_t utf8size = 3 * widesize + 1;
        resultstring.resize(utf8size, '\0');
        UTF16 * sourceUtf16 = new UTF16[widesize+1];
        for (size_t i=0; i<widesize; ++i)
            sourceUtf16[i] = (UTF16)widestring[i];
        sourceUtf16[widesize] = 0;

        const UTF16* sourcestart = sourceUtf16;
        const UTF16* sourceend = sourcestart + widesize;
        UTF8* targetstart = reinterpret_cast<UTF8*>(&resultstring[0]);
        UTF8* targetend = targetstart + utf8size;
        ConversionResult res = ConvertUTF16toUTF8(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
        delete[] sourceUtf16;
        if (res != conversionOK)
        {
            return "";
        }
        *targetstart = 0;
    }
    else if (sizeof(wchar_t) == 4)
    {
        size_t utf8size = 4 * widesize + 1;
        resultstring.resize(utf8size, '\0');
        UTF32 * sourceUtf32 = new UTF32[widesize+1];
        for (size_t i=0; i<widesize; ++i)
            sourceUtf32[i] = (UTF32)widestring[i];
        sourceUtf32[widesize] = 0;
        const UTF32* sourcestart = sourceUtf32;
        const UTF32* sourceend = sourcestart + widesize;
        UTF8* targetstart = reinterpret_cast<UTF8*>(&resultstring[0]);
        UTF8* targetend = targetstart + utf8size;
        ConversionResult res = ConvertUTF32toUTF8(&sourcestart, sourceend, &targetstart, targetend, strictConversion);
        delete[] sourceUtf32;
        if (res != conversionOK)
        {
            return "";
        }
        *targetstart = 0;
    }
    return std::string( resultstring.c_str() ); // this intentional! resultstring is different from the returned string.
}

void ToUpper(std::string &string_)
{
	const int length = string_.length();
	for ( int i=0; i!=length; ++i )
	{
		string_[i] = std::toupper(string_[i]);
	}
}

void ToLower(std::string &string_)
{
	const int length = string_.length();
	for ( int i=0; i!=length; ++i )
	{
		string_[i] = std::tolower(string_[i]);
	}
}

}; // namespace ExtStr


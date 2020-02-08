// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "helper_file.h"
#include "logging.h"

namespace ExtFile
{

void ErasePathFromFilename(std::string &filename_)
{
    std::string::size_type p1 = filename_.rfind("/");
    std::string::size_type p2 = filename_.rfind("\\");
    if ( p1 == std::string::npos || (p2 != std::string::npos && p2 > p1 ) )
        p1 = p2;
    if ( p1 != std::string::npos )
        filename_.erase(0, p1+1);
}

void ErasePathFromFilename(std::wstring &filename_)
{
    std::wstring::size_type p1 = filename_.rfind(L"/");
    std::wstring::size_type p2 = filename_.rfind(L"\\");
    if ( p1 == std::wstring::npos || (p2 != std::wstring::npos && p2 > p1 ) )
        p1 = p2;
    if ( p1 != std::wstring::npos )
        filename_.erase(0, p1+1);
}

void EraseFilenameFromPath(std::string &filename_)
{
    std::string::size_type p1 = filename_.rfind("/");
    std::string::size_type p2 = filename_.rfind("\\");
    if ( p1 == std::string::npos || (p2 != std::string::npos && p2 > p1 ) )
        p1 = p2;
    if ( p1 != std::string::npos )
        filename_.erase(p1+1, filename_.size());
    else
        filename_ += '/';
}

void EraseFilenameFromPath(std::wstring &filename_)
{
    std::wstring::size_type p1 = filename_.rfind(L"/");
    std::wstring::size_type p2 = filename_.rfind(L"\\");
    if ( p1 == std::wstring::npos || (p2 != std::wstring::npos && p2 > p1 ) )
        p1 = p2;
    if ( p1 != std::wstring::npos )
        filename_.erase(p1+1, filename_.size());
    else
        filename_ += L'/';
}

void EraseFiletypeFromFilename(std::string &filename_)
{
    std::string::size_type p1 = filename_.rfind(".");
    if ( p1 != std::string::npos )
        filename_.erase(p1, filename_.size());
}

void EraseFiletypeFromFilename(std::wstring &filename_)
{
    std::wstring::size_type p1 = filename_.rfind(L".");
    if ( p1 != std::wstring::npos )
        filename_.erase(p1, filename_.size());
}

// given "path/xyz/" and "xzz/abc" will result in "path/xyz/abc"
// Exception: when argv0_ contains a ':' it is an absolute path and will be used directly
std::string FindExePath(const std::string &cwd_, const std::string &argv0_)
{
    // is argv0_ an absolute windows path?
    std::string::size_type st = argv0_.find(":");
    if ( st != std::string::npos )
    {
        return argv0_;
    }

    // The merge position is the first position in cwd_ where the rest of
    // cwd_ does match the start of argv0_
    std::string::size_type mergePos = 0;
    while ( mergePos < cwd_.length() )
    {
        std::string::size_type len=(cwd_.length())-mergePos;
        if ( 0 == cwd_.compare(mergePos, len, argv0_, 0, len) )
        {
            break;
        }
        ++mergePos;
    }

    return cwd_.substr(0, mergePos) + argv0_;
}

void MakeLinuxTypePath(std::string &path_)
{
    LOG.DebugLn("MakeLinuxTypePath orig:", path_.c_str());
    bool lastCharWasSlash = false;
    for ( std::string::size_type t = path_.length()-1; ; --t )
    {
        if ( path_[t] == '\\' )
        {
            if ( lastCharWasSlash )
            {
                path_.erase(t, 1);
                lastCharWasSlash = false;
            }
            else
            {
                path_[t] = '/';
                lastCharWasSlash = true;
            }
        }
        else
        {
            lastCharWasSlash = false;
        }
        if ( t == 0 )
            break;
    }
    LOG.DebugLn("MakeLinuxTypePath modified:", path_.c_str());
}

// return charachters which will work on all supported systems
// (this means basicly windows 98)
char GetValidFilenameChar(char check_, bool allowPathChars_)
{
    if ( check_ >= 'a' && check_ <= 'z' )
        return check_;
    if ( check_ >= 'A' && check_ <= 'Z' )
        return check_;
    if ( check_ >= '0' && check_ <= '9' )
        return check_;

    if ( allowPathChars_ )
    {
        switch ( check_ )
        {
            case '\\':
            case '/':
            case ':':
                return check_;
            default:
                break;
        }
    }

    // some signs which seem to be ok, but it would be nice if i wouldn't just fix it for german
	const unsigned char AE = static_cast<unsigned char>(142);
	const unsigned char ae = static_cast<unsigned char>(132);
	const unsigned char OE = static_cast<unsigned char>(153);
	const unsigned char oe = static_cast<unsigned char>(148);
	const unsigned char UE = static_cast<unsigned char>(154);
	const unsigned char ue = static_cast<unsigned char>(129);
	const unsigned char ss = static_cast<unsigned char>(225);
    switch ( (unsigned char)check_ )
    {
        case AE:
        case ae:
        case OE:
        case oe:
        case UE:
        case ue:
        case ss:
            return check_;
        default:
            break;
    }

    switch ( check_ )
    {
        case '$':
        case '%':
        case '-':
        case '@':
        case '{':
        case '}':
        case '!':
        case '#':
        case '(':
        case ')':
        case '&':
        case '^':
        case '+':
        case ',':
        case '=':
        case '[':
        case ']':
        case '_':
        case '.':
        case '~':
        case '\'':
        case '`':
            return check_;
        default:
            return ('a'+(check_%26));
    }
    return check_;
}

}; // namespace ExtFile

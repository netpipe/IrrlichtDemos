/*!
	@file		IrrStringUtil.h
	@author		manguonden
	@date		11/2009
	@module
*//*
	This file is part of Irrlicht Engine.
	
	Irrlicht Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	Irrlicht Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with Irrlicht Engine.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef _StringUtil_H__
#define _StringUtil_H__

#include "IrrCompileConfig.h"
// Needed for IRR_WCHAR_T_STRINGS below
//#include <cstring>
//#include <cstdarg>
//#include <cmath>

// STL containers
//#include <vector>
//#include <map>
//#include <string>

// STL algorithms & functions
#include <algorithm>
//#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


namespace irr {

	/** If set to 1, Real is typedef'ed to double. Otherwise, Real is typedef'ed
    to float. Setting this allows you to perform mathematical operations in the
	CPU with more precision, but bear in mind that the
	GPU still operates in single-precision mode.
*/
#ifndef IRR_DOUBLE_PRECISION
#define IRR_DOUBLE_PRECISION 0
#endif

	 // define the real number values to be used
    // default to use 'float' unless precompiler option set
    #if IRR_DOUBLE_PRECISION == 1
		/** Software floating point type.
		@note Not valid as a pointer to GPU buffers / parameters
		*/
        typedef double Real;
    #else
		/** Software floating point type.
		@note Not valid as a pointer to GPU buffers / parameters
		*/
        typedef float Real;
    #endif

	// Integer formats of fixed bit width
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	// define uint64 type
	#if defined(_MSC_VER)
		typedef unsigned __int64 uint64;
	#else
		typedef unsigned long long uint64;
	#endif

	/** In order to avoid finger-aches :)
    */
    typedef unsigned char uchar;
    typedef unsigned short ushort;
    typedef unsigned int uint;
	typedef unsigned long ulong;

	#if IRR_WCHAR_T_STRINGS == 1
		typedef std::wstring _StringBase;
	#else
		typedef std::string _StringBase;
	#endif

	typedef _StringBase String;


    /** Utility class for manipulating Strings.  */
    class StringUtil
    {
	public:
        typedef std::ostringstream StrStreamType;

        /** Removes any whitespace characters, be it standard space or
            TABs and so on.
            @remarks
                The user may specify wether they want to trim only the
                beginning or the end of the String ( the default action is
                to trim both).
        */
        static void trim( String& str, bool left = true, bool right = true );

        /** Upper-cases all the characters in the string.
        */
        static void toLowerCase( String& str );

        /** Lower-cases all the characters in the string.
        */
        static void toUpperCase( String& str );


        /** Returns whether the string begins with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

        /** Returns whether the string ends with the pattern passed in.
        @param pattern The pattern to compare with.
        @param lowerCase If true, the end of the string will be lower cased before
            comparison, pattern should also be in lower case.
        */
        static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

        /** Method for standardising paths - use forward slashes only, end with slash.
        */
        static String standardisePath( const String &init);

        /** Method for splitting a fully qualified filename into the base name
            and path.
        @remarks
            Path is standardised as in standardisePath
        */
        static void splitFilename(const String& qualifiedName,
            String& outBasename, String& outPath);

		/** Method for splitting a fully qualified filename into the base name,
		extension and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFullFilename(const irr::String& qualifiedName, 
			irr::String& outBasename, irr::String& outExtention, 
			irr::String& outPath);

		/** Method for splitting a filename into the base name
		and extension.
		*/
		static void splitBaseFilename(const irr::String& fullName, 
			irr::String& outBasename, irr::String& outExtention);


        /** Simple pattern-matching routine allowing a wildcard pattern.
        @param str String to test
        @param pattern Pattern to match against; can include simple '*' wildcards
        @param caseSensitive Whether the match is case sensitive or not
        */
        static bool match(const String& str, const String& pattern, bool caseSensitive = true);





        /// Constant blank string, useful for returning by ref where local does not exist
        static const String BLANK;
    };


} // End of  irr namespace section

#endif // _StringUtil_H__

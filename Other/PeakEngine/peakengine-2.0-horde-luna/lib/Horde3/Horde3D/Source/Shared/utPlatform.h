// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _utPlatform_H_
#define _utPlatform_H_

#ifndef PLATFORM_WIN
#	if defined( WIN32 ) || defined( _WINDOWS )
#		define PLATFORM_WIN
#	endif
#endif

#ifndef PLATFORM_MAC
#   if defined( __APPLE__ ) || defined( __APPLE_CC__ )
#      define PLATFORM_MAC
#   endif
#endif



#ifndef DLLEXP
#	ifdef PLATFORM_WIN
#		define DLLEXP extern "C" __declspec( dllexport )
#	else
#		define DLLEXP extern "C"
#	endif
#endif

#ifndef PLATFORM_WIN
#	define _stricmp strcasecmp
#	define _mkdir( name ) mkdir( name, 0755 )
#endif

// The following lines will produce a compiler error if integer types have unexpected sizes
typedef unsigned char validate_int32[sizeof( int ) == 4];
typedef unsigned char validate_int16[sizeof( short ) == 2];
typedef unsigned char validate_int8[sizeof( char ) == 1];

#endif // _utPlatform_H_

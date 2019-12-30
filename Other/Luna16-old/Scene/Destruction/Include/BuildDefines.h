/*
===============================================================================
	File:	BuildDefines.h
	Desc:	Build options, preprocessor settings for compiling different versions.
===============================================================================
*/

#ifndef __MX_BUILD_DEFINES_H__
#define __MX_BUILD_DEFINES_H__

//#define MX_DEBUG		// Debug build.
#define MX_DEVELOPER	// For statistics, testing, etc.
#define MX_SOUND		// Compile with sound.

#define BULLET_PHYSICS	// Use Bullet physics engine.

//----------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

// Nonstandard extension: enum used in qualified name.
#pragma warning ( disable: 4482 )

// Deprecation warning: this function or variable may be unsafe.
#pragma warning ( disable: 4996 )

#endif // !__MX_BUILD_DEFINES_H__

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//

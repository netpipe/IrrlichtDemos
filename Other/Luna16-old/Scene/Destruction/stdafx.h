// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __MX_PRECOMPILED_H__
#define __MX_PRECOMPILED_H__

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

// Build options.
#include "Include/BuildDefines.h"

#ifndef MX_DEBUG
	#undef _DEBUG
#endif //!MX_DEBUG

//-----------------------------------------------
// C run-time library.
#include <stdio.h>
//#include <tchar.h>
#include <string.h>
#include <assert.h>
#include <memory>

//-----------------------------------------------
// STL
#include <map>

//-----------------------------------------------
//#include <windows.h>

//#include <d3d9.h>
//#include <d3dx9.h>

//#pragma comment( lib, "d3d9.lib" )
//#pragma comment( lib, "d3dx9.lib" )

//---- Graphics engine ---------------------------
//
#include <irrlicht.h>
#pragma comment( lib, "Irrlicht.lib" )

//---- Sound engine ------------------------------
//
#ifdef MX_SOUND
//	#include <irrKlang.h>
//	#pragma comment( lib, "irrKlang.lib" )
#endif //MX_SOUND

//---- Physics engine ------------------------------
//
#include <btBulletDynamicsCommon.h>
#pragma comment( lib, "libbulletmath.lib" )
#pragma comment( lib, "libbulletcollision.lib" )
#pragma comment( lib, "libbulletdynamics.lib" )

#endif /* ! __MX_PRECOMPILED_H__ */

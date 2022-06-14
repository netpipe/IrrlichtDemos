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



//-----------------------------------------------
// C run-time library.
#include <stdio.h>
//#include <tchar.h>
#include <string.h>
#include <assert.h>
#include <memory>

#include <map>


//---- Graphics engine ---------------------------
//
#include <irrlicht.h>



//---- Physics engine ------------------------------
//
#include <btBulletDynamicsCommon.h>


#endif /* ! __MX_PRECOMPILED_H__ */

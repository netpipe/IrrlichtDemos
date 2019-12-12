/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently



#if !defined(AFX_STDAFX_H__AE78B9E2_A5B8_11D4_A1FB_00500C0076C8__INCLUDED_)
#define AFX_STDAFX_H__AE78B9E2_A5B8_11D4_A1FB_00500C0076C8__INCLUDED_

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


typedef char dInt8;
typedef unsigned char dUnsigned8;

typedef short dInt16;
typedef unsigned short dUnsigned16;

typedef int dInt32;
typedef unsigned dUnsigned32;
typedef unsigned int dUnsigned32;

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <ctype.h>



#ifdef _MSC_VER
	#include <windows.h>
	#include <crtdbg.h>
	#include <GL/glew.h>
	#include <GL/wglew.h>
	#include <wx/wx.h>
	#include <wx/dcclient.h>
	#include <wx/glcanvas.h>
	#include <wx/event.h>
	#include <malloc.h>
#endif
	
#ifdef _LINUX_VER
	#include <unistd.h>
	#include <time.h>
	#include <GL/glew.h>
	#include <wx/wx.h>
	#include <wx/dcclient.h>
	#include <wx/glcanvas.h>
	#include <wx/event.h>
#endif

#ifdef _MAC_VER
	#include <CoreFoundation/CoreFoundation.h> 
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
//	#include <GLEW/glew.h>
	#include <wx/wx.h>
	#include <wx/dcclient.h>
	#include <wx/glcanvas.h>
	#include <wx/event.h>
#endif
	


#include <Newton.h>

// SDK include
#include <dVector.h>
#include <dMatrix.h>
#include <dQuaternion.h>
#include <dMathDefines.h>
//#include <CustomJointLibraryStdAfx.h>
//#include <Newton.h>




#ifdef __USE_DOUBLE_PRECISION__
	#define glMultMatrix(x) glMultMatrixd(x)
	#define glLoadMatrix(x) glMultMatrixd(x)
//	#define glGetFloat(x,y) glGetDoublev(x,(GLdouble *)y) 
#else
	#define glMultMatrix(x) glMultMatrixf(x)
	#define glLoadMatrix(x) glMultMatrixf(x)
	#define glGetFloat(x,y) glGetFloatv(x,(dFloat  *)y) 
#endif


#ifdef _MSC_VER
//	#pragma warning (disable: 4100) //unreferenced formal parameter
	#pragma warning (disable: 4505) //unreferenced local function has been removed
	#pragma warning (disable: 4201) //nonstandard extension used : nameless struct/union
	#pragma warning (disable: 4127) //conditional expression is constant

	#if (_MSC_VER >= 1400)
		#pragma warning (disable: 4996) // for 2005 users declared deprecated
	#endif

#endif



#ifndef strlwr
	inline char *_strlwr_ (char *a) 
	{ 
		char *ret = a; 
		while (*a != '\0') 
		{ 
			//if (isupper (*a)) 
			*a = char (tolower (*a)); 
			++a; 
		} 
		return ret; 
	}

	#define strlwr(a) _strlwr_ (a) 
#endif


#ifndef min
#define min(a,b) ((a < b) ? a : b)
#endif

#ifndef max
#define max(a,b) ((a > b) ? a : b)
#endif



// for some reason specifying a relative does not seem to work in Linus
// and i have to specify a absolute path
// #define ASSETS_PATH "."
void GetWorkingFileName (const char* name, char* outPathName);


// little Indian/big Indian conversion
unsigned SWAP_INT32(unsigned x);
unsigned short SWAP_INT16(unsigned short x);
void SWAP_FLOAT32_ARRAY (void *array, dInt32 count);



#endif 


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


typedef long long unsigned64;


#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <ctype.h>



#ifdef _MSC_VER
	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	#include <windows.h>
	#include <Commctrl.h>
	#include <stdlib.h>
	#include <malloc.h>
	#include <memory.h>
	#include <time.h>
	#include <tchar.h>
	#include <crtdbg.h>
	#include <GL/glew.h>
	#include <GL/wglew.h>
	#include <gl/gl.h>
	#include <gl/glu.h>
#endif
	
#ifdef _LINUX_VER
	#include <unistd.h>
	#include <time.h>
	#include <GL/glew.h>
#endif

#ifdef _MAC_VER
	#include <CoreFoundation/CoreFoundation.h> 
	#include <unistd.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
//	#include <GLEW/glew.h>
#endif


#ifdef _MSC_VER
	#pragma warning (disable: 4100) //unreferenced formal parameter
	#pragma warning (disable: 4505) //unreferenced local function has been removed
	#pragma warning (disable: 4201) //nonstandard extension used : nameless struct/union
	#pragma warning (disable: 4127) //conditional expression is constant

	#if (_MSC_VER >= 1400)
		#pragma warning (disable: 4996) // for 2005 users declared deprecated
	#endif
#endif


#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtCore/QVariant>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QFont>
#include <QAbstractEventDispatcher>

//#include <QtGui/QSplitter>
//#include <QtGui/QStatusBar>
//#include <QtGui/QFileDialog>
//#include <QtGui/QButtonGroup>
//#include <QtGui/QHeaderView>
//#include <QtGui/QToolBar>
//#include <QtGui/QWidget>
//#include <QtGui/QIcon>
//#include <QtGui/QPixmap>
//#include <QtGui/QBitmap>
//#include <QtGui/QTextEdit>
//#include <QtGui/QResizeEvent>
//#include <QtGui/QHBoxLayout>
//#include <QtOpenGL/QGLWidget>
//#include <QtCore/QList>



#include <Newton.h>

// SDK include
#include <dVector.h>
#include <dMatrix.h>
#include <dQuaternion.h>
#include <dMathDefines.h>
//#include <CustomJointLibraryStdAfx.h>
//#include <Newton.h>

#include <dCRC.h>
#include <dHeap.h>
#include <dList.h>
#include <dTree.h>
#include <dRtti.h>
#include <dThread.h>
#include <dClassInfo.h>
#include <dRefCounter.h>
#include <dBaseHierarchy.h>


#include "dPluginStdafx.h"
#include "dScene.h"
#include "dCameraNodeInfo.h"
#include "dRootNodeInfo.h"
#include "dBoneNodeInfo.h"
#include "dSceneNodeInfo.h"
#include "dMeshNodeInfo.h"
#include "dTextureNodeInfo.h"
#include "dMaterialNodeInfo.h"
#include "dRigidbodyNodeInfo.h"
#include "dCollisionNodeInfo.h"
#include "dCollisionBoxNodeInfo.h"
#include "dCollisionSphereNodeInfo.h"
#include "dCollisionConvexHullNodeInfo.h"
#include "dGeometryNodeSkinModifierInfo.h"




#ifdef __USE_DOUBLE_PRECISION__
	#define glMultMatrix(x) glMultMatrixd(x)
	#define glLoadMatrix(x) glMultMatrixd(x)
//	#define glGetFloat(x,y) glGetDoublev(x,(GLdouble *)y) 
#else
	#define glMultMatrix(x) glMultMatrixf(x)
	#define glLoadMatrix(x) glMultMatrixf(x)
	#define glGetFloat(x,y) glGetFloatv(x,(dFloat  *)y) 
#endif



#ifndef _MSC_VER
	#ifndef stricmp
		#define stricmp strcasecmp
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
void GetWorkingFileName (const char* const name, char* const outPathName);


// little Indian/big Indian conversion
unsigned SWAP_INT32(unsigned x);
unsigned short SWAP_INT16(unsigned short x);
void SWAP_FLOAT32_ARRAY (void *array, dInt32 count);



#endif 


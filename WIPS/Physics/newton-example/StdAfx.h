// stdafx.h : used by visual c. sorry :p
//

#if !defined(AFX_STDAFX_H__A2BDE55A_D9D6_448F_8A27_9F660C8D24D4__INCLUDED_)
#define AFX_STDAFX_H__A2BDE55A_D9D6_448F_8A27_9F660C8D24D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "Newton.lib")

#define NEWTON_GRAVITY -800.0f

#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>

// irrlicht
#include <irrlicht.h>
#include <wchar.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// newton
#include <newton/Newton.h>

// example class
#include "game.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A2BDE55A_D9D6_448F_8A27_9F660C8D24D4__INCLUDED_)


#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef WIN32
	#include <windows.h>
	#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

extern IrrlichtDevice *device;
extern IVideoDriver *driver;

#endif //_COMMON_H_

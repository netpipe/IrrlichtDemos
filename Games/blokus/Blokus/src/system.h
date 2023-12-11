
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "common.h"
#include "resource.h"

#ifdef WIN32
	#include <mmsystem.h>
	#pragma comment(lib, "winmm.lib")
#else
	#define HINSTANCE s32
#endif

void sys_setIcon(HINSTANCE hInst);
void sys_setCamera(IVideoDriver	*driver, ISceneManager *manager, bool bOrtho);
bool sys_checkFps(s32 frame);

#endif //_SYSTEM_H_


#ifndef _CDRAW_H_
#define _CDRAW_H_

//#include "common.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;

#define PI 3.141592654

#define PS_SOLID            0       // ëSÇƒìhÇËÇ¬Ç‘Çµ
#define PS_DASH             1       // -------  
#define PS_DOT              2       // .......  
#define PS_DASHDOT          3       // _._._._  
#define PS_DASHDOTDOT       4       // _.._.._  
#define PS_NULL             5       // ñ‘ñ⁄
#define PS_INSIDEFRAME      6       // ògÇï`âÊ
#define PS_USERSTYLE        7       // ì¡éÍâ°é» 
#define PS_SPECIAL          8       // ì¡éÍècâ°é» 

#define Color unsigned long

typedef struct
{
	int x,y,w,h;
}Rect;

typedef struct
{
	int x,y;
}Pos;

bool drawInit(IrrlichtDevice *device, IVideoDriver* driver);
bool fontInit(IrrlichtDevice *device, IVideoDriver* driver);
void makePattern(IVideoDriver* driver);
dimension2d<s32> GetImageSize(IVideoDriver* driver,char *filename);
void DrawImage(IVideoDriver* driver, char* filename, int x, int y, int w, int h, int u, int v, 
		bool bColorKey = false, SColor colLU=0xFFFFFFFF, SColor colLD=0, SColor colRD=0, SColor colRU=0,float sx=1.0f, float sy=1.0f);
void DrawLine(IVideoDriver* driver, int x1, int y1, int x2, int y2, SColor col);
void DrawRect(IVideoDriver* driver, int x, int y, int w, int h, SColor col);
void FillRect(IVideoDriver* driver, int x, int y, int w, int h, SColor col, int type = PS_SOLID);
void FillRect2(IVideoDriver* driver, int x, int y, int w, int h, 
		SColor colLU, SColor colLD, SColor colRD, SColor colRU, int type = PS_SOLID);
void DrawOval(IVideoDriver* driver, int cx, int cy, float radius, SColor col, int count);
void FillOval(IVideoDriver* driver, int cx, int cy, float radius, SColor col, int count);
position2d<s32>  Printf(IVideoDriver* driver, int x, int y, Rect *pRect, SColor color, const char *fmt, ...);
position2d<s32>  bold(IVideoDriver* driver, int x, int y, Rect *pRect, SColor color, const char *fmt, ...);
void sysPrintf(int x, int y, SColor color, const char *fmt, ...);
char* utf16be_to_sjis(const wchar_t *pUcsStr, int *nBytesOut);
wchar_t* sjis_to_utf16be(const char *pAnsiStr, int *nBytesOut);

#endif //_CDRAW_H_

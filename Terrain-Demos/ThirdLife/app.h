#ifndef APP_HEADER
#define APP_HEADER

#include <irrlicht.h>
#include <math.h>

#include "EventReceiver.h"
#include "EditableTerrain.h"
#include "GUIManager.h"
#include "InputHandler.h"
#include "Player.h"

inline float round( float f ){ return (f > 0.0) ? (float)floor(f + 0.5) : (float)ceil(f - 0.5); }

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif //_IRR_WINDOWS_

#endif //APP_HEADER
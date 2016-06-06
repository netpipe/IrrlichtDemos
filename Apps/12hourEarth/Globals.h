
#ifndef GLOBALS_H
#define GLOBALS_H
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;
GameClock gameclock;
irr::f32 gamespeed = 1.0f;
irr::ITimer *timer;

#endif 

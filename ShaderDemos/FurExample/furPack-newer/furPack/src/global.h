#ifndef GLOBAL_H
#define GLOBAL_H

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class MyShaderCallBack;
extern bool blShader;

#pragma comment(lib, "Irrlicht.lib")

extern IrrlichtDevice *device;
extern IVideoDriver* driver;
extern ISceneManager* smgr;
extern IGPUProgrammingServices* gpu;

extern MyShaderCallBack* ShaderCB;
extern s32 EMT_FUR;

#endif

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib,"irrlicht.lib")

extern IVideoDriver			   * pVideo;
extern ISceneManager		   * pScene;
extern IrrlichtDevice		   * device;
extern IGUIEnvironment		   * pGUI;
extern IGPUProgrammingServices * pGPU;
extern ILogger				   * pLog;
extern ISceneCollisionManager  * pCollision;
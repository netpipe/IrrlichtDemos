/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com) */


// not working at the moment, though the same code runs in the engine.
#include "cOpenSteerDemo.h"
#undef useIrrExtensions13
#include <irrlicht.h>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
using namespace irr;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice  *device;
IVideoDriver    *driver;
ISceneManager   *smgr;
IGUIEnvironment *guienv;

void main_loop(){
  while(device->run()){

      driver->beginScene(true, true, SColor(0, 0, 0, 0));
	        OpenSteer::OpenSteerDemo::updateSimulationAndRedraw();

      smgr->drawAll();
    //OpenSteer::runGraphics();


	guienv->drawAll();

	//manager->drawAll();
	driver->endScene();

 //OpenSteer::OpenSteerDemo::updateSimulationAndRedraw();
  device->sleep(10);
  }
};


class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


int main(int argc, char **argv){

MyEventReceiver receiver;
#ifdef __EMSCRIPTEN__
    irr::SIrrlichtCreationParameters irrParams = SIrrlichtCreationParameters();
    params.DriverType = video::EDT_OGLES2;
    params.WindowSize = dimension2d<u32>(640, 480);
    params.Bits = 16;
    params.Fullscreen = false;
    params.Stencilbuffer = false;
    params.Vsync = false;
    params.EventReceiver = 0;
    params.OGLES2ShaderPath = std::string("media/Shaders/").c_str();
    device = createDeviceEx(params);
    #else
      irr::SIrrlichtCreationParameters irrParams;
  irrParams.DriverType = irr::video::EDT_OPENGL;
  irrParams.EventReceiver = &receiver;
  irrParams.Fullscreen = false;
  irrParams.Bits = 16;
  irrParams.Vsync = false;
  irrParams.WindowSize = irr::core::dimension2d<u32>(640, 480);

#endif
//
//	device =
//		createDevice( video::EDT_OPENGL, dimension2du(640, 480), 16,
//			false, false, false, 0);
//	IrrlichtDevice* device = createDevice(irr::video::EDT_OPENGL,
//			core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

      device = createDeviceEx(irrParams);
  driver = device->getVideoDriver();
  smgr   = device->getSceneManager();
  guienv = device->getGUIEnvironment();


	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

  // initialize graphics first !!!
  OpenSteer::initializeGraphics(device);
  // initialize OpenSteerDemo application
  OpenSteer::OpenSteerDemo::initialize();
  OpenSteer::OpenSteerDemo::selectNextPlugIn();
  OpenSteer::OpenSteerDemo::selectNextPlugIn();
  OpenSteer::OpenSteerDemo::selectNextPlugIn();
OpenSteer::runGraphics();
//  while(device->run()){
//      driver->beginScene(true, true, SColor(0, 0, 0, 0));
//	        OpenSteer::OpenSteerDemo::updateSimulationAndRedraw();
//      smgr->drawAll();
//	guienv->drawAll();
//	smgr->drawAll();
//	driver->endScene();
//
//// OpenSteer::OpenSteerDemo::updateSimulationAndRedraw();
//  device->sleep(1);
//  }
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
 //  run main loop
// main_loop();
  //OpenSteer::runGraphics();
#endif

  return 0;
}

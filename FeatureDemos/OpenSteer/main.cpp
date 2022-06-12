/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com) */

#include "cOpenSteerDemo.h"
#undef useIrrExtensions13
#include <irrlicht.h>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look at them, you can
read a detailed description of them in the documentation by clicking on the top
menu item 'Namespace List' or by using this link:
http://irrlicht.sourceforge.net/docu/namespaces.html
Like the irr namespace, we do not want these 5 sub namespaces now, to keep this
example simple. Hence, we tell the compiler again that we do not want always to
write their names.
*/
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
  device->run();
      driver->beginScene(true, true, SColor(0, 0, 0, 0));
        OpenSteer::OpenSteerDemo::updateSimulationAndRedraw();
      smgr->drawAll();
    //OpenSteer::runGraphics();


	guienv->drawAll();
	//manager->drawAll();
	driver->endScene();
  device->sleep(10);
};

int main(int argc, char **argv){
//
//  irr::SIrrlichtCreationParameters irrParams;
//  irrParams.DriverType = irr::video::EDT_OGLES2;
//  irrParams.EventReceiver = 0;
//  irrParams.Fullscreen = false;
//  irrParams.Bits = 16;
//  irrParams.Vsync = false;
//  irrParams.WindowSize = irr::core::dimension2du(800,600);

    irr::SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
   /// params.DriverType = video::EDT_OGLES2;
    params.DriverType = video::EDT_OPENGL;
    params.WindowSize = dimension2d<u32>(640, 480);
    params.Bits = 16;
    params.Fullscreen = false;
    params.Stencilbuffer = false;
    params.Vsync = false;
    params.EventReceiver = 0;
    ///params.OGLES2ShaderPath = std::string("media/Shaders/").c_str();
    device = createDeviceEx(params);


//      device = createDeviceEx(deviceParams);
  driver = device->getVideoDriver();
  smgr   = device->getSceneManager();
  guienv = device->getGUIEnvironment();


//	device =
//		createDevice( video::EDT_OGLES2, dimension2du(640, 480), 16,
//			false, false, false, 0);

  // initialize graphics first !!!
  OpenSteer::initializeGraphics(device);
  // initialize OpenSteerDemo application
  OpenSteer::OpenSteerDemo::initialize();

  OpenSteer::OpenSteerDemo::selectNextPlugIn();
  OpenSteer::OpenSteerDemo::selectNextPlugIn();
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
 //  run main loop
  OpenSteer::runGraphics();
#endif

  return 0;
}

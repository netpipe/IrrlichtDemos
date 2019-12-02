#include <irrlicht.h>
#include "driverChoice.h"
#include <iostream>
#include <string>
#include <sstream>
#include "TiledTerrain.h"

using namespace irr;
using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

/********************** CLASS MyEventReceiver **********************/
class MyEventReceiver : public IEventReceiver
{
public:

  MyEventReceiver(TiledTerrain* terrain, scene::ISceneNode* skydome, scene::ICameraSceneNode* camera, scene::ISceneManager* smgr )
  {
    this->Terrain = terrain;
    this->Skydome=skydome;
    this->Camera=camera;
    this->Smgr=smgr;
    this->showDebug = false;
    Skydome->setVisible(true);
  }

  void redoCameraHeight(f32 height)
  {
    Camera->setPosition(core::vector3df(Camera->getAbsolutePosition().X,
					Terrain->getCurrentTerrain()->getHeight(Camera->getAbsolutePosition().X,Camera->getAbsolutePosition().Z)+height,
					Camera->getAbsolutePosition().Z));
  }

  void redoCameraSpeed(f32 speed)
  {
    core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator anims=Camera->getAnimators().begin();
    irr::scene::ISceneNodeAnimatorCameraFPS *anim=(irr::scene::ISceneNodeAnimatorCameraFPS*)*anims;
    anim->setMoveSpeed(speed);
  }

  bool OnEvent(const SEvent& event)
  {
    // check if user presses the key 'W' or 'D'
    if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
      {
	switch (event.KeyInput.Key)
	  {
	  case irr::KEY_KEY_W: // switch wire frame mode
	    Terrain->getCurrentTerrain()->setMaterialFlag(video::EMF_WIREFRAME,
							  !Terrain->getCurrentTerrain()->getMaterial(0).Wireframe);
	    Terrain->getCurrentTerrain()->setMaterialFlag(video::EMF_POINTCLOUD, false);
	    return true;
	  case irr::KEY_KEY_P: // switch wire frame mode
	    Terrain->getCurrentTerrain()->setMaterialFlag(video::EMF_POINTCLOUD,
							  !Terrain->getCurrentTerrain()->getMaterial(0).PointCloud);
	    Terrain->getCurrentTerrain()->setMaterialFlag(video::EMF_WIREFRAME, false);
	    return true;
	  case irr::KEY_KEY_D: // toggle detail map
	    Terrain->getCurrentTerrain()->setMaterialType(
							  Terrain->getCurrentTerrain()->getMaterial(0).MaterialType == video::EMT_SOLID ?
				     video::EMT_DETAIL_MAP : video::EMT_SOLID);
	    return true;
	  case irr::KEY_KEY_X: // toggle debug information
	    showDebug=!showDebug;
	    Terrain->getCurrentTerrain()->setDebugDataVisible(showDebug?scene::EDS_BBOX_ALL:scene::EDS_OFF);
	    return true;
	  case irr::KEY_KEY_1: // move camera 2m above terrain
	    redoCameraHeight(20);
	    return true;
	  case irr::KEY_KEY_2: // move camera 10m above terrain
	    redoCameraHeight(100);
	    return true;
	  case irr::KEY_KEY_3: // move camera 5.000feet above terrain
	    redoCameraHeight(15240);
	    return true;
	  case irr::KEY_KEY_4: // move camera 20.000feet above terrain
	    redoCameraHeight(60960);
	    return true;
	  case irr::KEY_KEY_5: // move camera 40.000feet above terrain
	    redoCameraHeight(121920);
	    return true;
	  case irr::KEY_KEY_6: // Camera speed = 0.1
	    redoCameraSpeed(0.1f);
	    return true;
	  case irr::KEY_KEY_7: // Camera speed = 1
	    redoCameraSpeed(1.0f);
	    return true;
	  case irr::KEY_KEY_8: // Camera speed = 20
	    redoCameraSpeed(20.0f);
	    return true;
	  case irr::KEY_KEY_9: // Camera speed = 50
	    redoCameraSpeed(50.0f);
	    return true;
	  case irr::KEY_KEY_Q: // quit
	    exit(0);
	  default:
	    break;
	  }
      }
    return false;
  }

private:
  TiledTerrain* Terrain;
  scene::ICameraSceneNode* Camera;
  scene::ISceneNode* Skydome;
  bool showDebug;
  scene::ISceneManager* Smgr;
};


/********************** Main **********************/
int main()
{

  int num_layers = 2;
  f32 tile_size = 128.f*900.f;

  // ask user for driver
  video::E_DRIVER_TYPE driverType=driverChoiceConsole();
  if (driverType==video::EDT_COUNT)
    return 1;

  // device
  irr::SIrrlichtCreationParameters params;
  params.DriverType=driverType;
  params.WindowSize=core::dimension2d<u32>(800, 600);
  params.Fullscreen= false;
  //  IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);// create a NULL device to detect screen resolution
  //  params.WindowSize=nulldevice->getVideoModeList()->getDesktopResolution();
  //  params.Fullscreen= true;
  IrrlichtDevice* device = createDeviceEx(params);
  if (device == 0)
    return 1;
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
  gui::IGUIEnvironment* env = device->getGUIEnvironment();
  device->getCursorControl()->setVisible(false);  // disable mouse cursor

  // Fog
  driver->setFog(video::SColor(0,150,150,160), irr::video::EFT_FOG_LINEAR, 150000, 200000, 0.6f, false, false);

  // Camera
  scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,250.0f,20.0f);
  camera->setPosition(core::vector3df((-1)*tile_size*(num_layers-1)-57000,3000,tile_size*(num_layers)+57000)); // Camera set at center of terrain
  camera->setTarget(core::vector3df(0,0,0));
  camera->setFarValue(5000000.0f);
  camera->setNearValue(1.0f);

  // skydome
  scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("./skydome.jpg"),16,8,0.85f,2.0f);

  // Terrain
  TiledTerrain terrain(139, //MaxRow,
		       92, //MaxColumn,
		       num_layers, //num_layers
		       device,
		       smgr,
		       driver,
		       40, //initial_row,
		       62, //initial_column,
		       tile_size,
		       900.f, //scale XZ
		       180.f, //scale Y
		       10000.f //margin = 1km
		       );
  // scale=900.f : heightmap has resolution of 90m. with a scale of 900, each irrlicht unit will represent 1dm
  // scale=180.f : The highest point on the heightmap is 4590m and we have 255 colors available.
  // Each color represents jump of 18m = 180dm = 180.f irrlicht units.

  // event receiver
  MyEventReceiver receiver(&terrain, skydome, camera, smgr);
  device->setEventReceiver(&receiver);


  /*
  driver->beginScene(true, true, video::SColor(0,0,50,0) );
  smgr->drawAll();
  env->drawAll();
  driver->endScene();
  */
  int lastFPS = -1;

  while(device->run())
    if (device->isWindowActive())
      {

	driver->beginScene(true, true, video::SColor(0,0,50,0) );
	smgr->drawAll();
	env->drawAll();
	driver->endScene();

	terrain.shiftTerrain(camera);

	// altitude
	int fps = driver->getFPS();
	if (lastFPS != fps or true)
	  {
	    core::stringw str = L"[";
	    str += "FPS:";
	    str += fps;
	    str += "] X: ";
	    str += camera->getAbsolutePosition().X;
	    str += " Y: ";
	    str += camera->getAbsolutePosition().Y;
	    str += " Z: ";
	    str += camera->getAbsolutePosition().Z;
	    device->setWindowCaption(str.c_str());
	    lastFPS = fps;
	  }
	  device->sleep(5);
      }
  device->drop();
  return 0;
}

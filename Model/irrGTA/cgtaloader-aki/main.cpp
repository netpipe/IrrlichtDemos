#include "cGTALoader.h"
#include "irrlicht.h"

int main(){
   IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);
   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();
   device->setWindowCaption(L"GTA-Loader");

  // for easyness I packed all mesh and texture files in an archive
  device->getFileSystem()->addZipFileArchive("gta3img.zip");

  // load a map via ipl file
  cGTALoader gtaLoader(device);
  ISceneNode* map = gtaLoader.loadMap("tropic/tropic.ipl", gta3);
  map->setRotation(vector3df(-90,0,0));
  map->setScale(vector3df(2,2,2));

   // setup and run Irrlicht render loop
  smgr->addCameraSceneNodeFPS(0,50,50);
  device->getCursorControl()->setVisible(false);
   while(device->run()){
      driver->beginScene(true, true, video::SColor(0,0,0,200));
      smgr->drawAll();
      driver->endScene();
   }
  device->drop();
   return 0;
}

#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <driverChoice.h>

#include "Application.h"
#include "../enviroment/Sky.h"
#include "Logger.h"
#include "bullethelper.h"
using namespace irr;
Application::Application(){
  // ask user for driver
   video::E_DRIVER_TYPE driverType = driverChoiceConsole();
   if (driverType==video::EDT_COUNT)
       exit (EXIT_FAILURE);

  this->device = createDevice(driverType, core::dimension2d<u32>(800,600), 16, false, false, false, 0);
  this->device->getLogger()->setLogLevel(ELL_NONE);

    device->getFileSystem()->addFileArchive("./media.zip");
  #if (DEBUG_OUTPUT_MASK & 2)
      // this->device->getLogger()->setLogLevel(ELL_DEBUG);
  #endif
  this->driver = this->device->getVideoDriver();
  this->driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
  this->smgr = this->device->getSceneManager();
  this->smgr->setAmbientLight(video::SColor(0,255,255,204));
  log(INFO,1,"Turn on lights");
  this->gui = this->device->getGUIEnvironment();
  this->createWorldEnviroment();
}
void Application::createWorldEnviroment(){
  core::stringw pathTexture[2] = {"media/terrain/grass_green_old.jpg","media/terrain/grass_green_thin.jpg"};
  this->terrain = new Terrain(smgr,driver,pathTexture,core::vector3df(250.f,0.0001f, 250.f));
  Sky(smgr,driver,"media/terrain/sky.jpg",1000.0f);
  log1("World created");
  this->physics = new Physics(terrain->getTerrainBox());
  log(INFO,1,"Law of physics successfully created");
}
bool Application::init(){
  this->screen = new Launcher(this->gui, this->device);
  assert(screen != 0);
  while(device->run() && driver){
   if (device->isWindowActive()){
      driver->beginScene(true, true, video::SColor(255,200,200,200));
       this->gui->drawAll();
       assert(screen->listener != 0);
       if(screen->listener->stateElement[GUI_ID_START_BUTTON]){
         break;
         screen->listener->stateElement[GUI_ID_START_BUTTON] = false;
       }
       driver->endScene();
   }
 }
 if(!driver) return false;
  // this->names = new core::stringw[2];
  core::stringw* names = new core::stringw[2];
  names[0] = (screen->players[0]->getText());
  names[1] = (screen->players[1]->getText());
  screen->clear();
  this->gameManager = new GameManager(smgr,driver,device,terrain->getCenter(), physics, names);
  #if (DEBUG_OUTPUT_MASK & 2)
      this->smgr->addCameraSceneNodeFPS(0,100.f,0.04f)->setPosition(terrain->getCenter());
  #endif

  this->device->getCursorControl()->setVisible(false);
  log1("Gameplay initialized.");

  return true;
}
bool Application::end(){
    this->screen->endScreen(this->gameManager->getNames(), this->gameManager->getWinner());
    driver->beginScene(true, true, video::SColor(255,200,200,200));
    this->gui->drawAll();
    driver->endScene();
   //  while(device->run() && driver){
   //   if (device->isWindowActive()){
   //   }
   //   break;
   // }
   /** Free memory */
   /**
   * From Irrlicht docs:.
   * This method can free a lot of memory!
   * Please note that after calling this,
   * the pointer to the ITexture may no longer be valid, if it was not grabbed before by other parts of the engine for storing it longer.
   * So it is a good idea to set all materials which are using this texture to 0 or another texture first.
   */
   this->driver->removeAllTextures();
   /** Clear every scene node */
   this->smgr->clear();
   /** Remove bullet memory  */
   delete this->physics;
   int exit;
   std::cout<<"Memory liberated"<<std::endl;
   std::cout<<"Press something to close game"<<std::endl;
   std::cin>>exit;
   this->gui->clear();
   return 0;
}
bool Application::loop(){
  u32 TimeStamp = device->getTimer()->getTime(), DeltaTime = 0;
  u32 lastFPS = 0;
  bool flagWin = false;
  while(device->run() && device) {

    //        if(device->isWindowActive()){
                u32 fps = driver->getFPS();
                DeltaTime = device->getTimer()->getTime() - TimeStamp;
                physics->UpdatePhysics(DeltaTime,fps);
                driver->beginScene(true, true, video::SColor(255,200,200,200));

                smgr->drawAll();
                if(gameManager->loop()){
                    flagWin = true;
                    break;
                }
                driver->endScene();

                TimeStamp = device->getTimer()->getTime();

                if(lastFPS != fps)
                {
                    core::stringw str =L"Castle Battle [";
                    str += driver->getName();
                    str += "] FPS:";
                    str += fps;
                    device->setWindowCaption(str.c_str());
                    lastFPS = fps;
                }
  }
  if(flagWin) {
      std::cout<<"Thanks for playing. Goodbye!"<<std::endl;
      this->end();
  }
  return 1;

}

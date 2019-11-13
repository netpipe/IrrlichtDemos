  #include <irrlicht.h>
  #include <IrrOde.h>
  #include <IState.h>
  #include <CMenu.h>
  #include <CPlay.h>
  #include <CSettings.h>
  #include <CAbout.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char** argv)
{
IrrlichtDevice *device=NULL;
array<IState *> pStates;
IState *pActive;
u32 iActive=0,iRet=0;
bool bQuit=false;

  //First thing to do: show the graphics options dialog to let the user choose the graphics options
  CSettings *pSettings=new CSettings("../../data/irrOdeMarble_setup.xml","../../data/irrOdeMarbleSettings.xml",L"irrOdeMarble - Graphics Setup",SColor(0x00,0x21,0xAD,0x10));

  iRet=pSettings->run();
  if (iRet==2) {
    delete pSettings;
    return 0;
  }

  //now create the Irrlicht device from the chosen options
  device=pSettings->createDeviceFromSettings();

  //get pointers to the usual objects
  IVideoDriver    *driver=device->getVideoDriver   ();
  ISceneManager   *smgr  =device->getSceneManager  ();
  IGUIEnvironment *guienv=device->getGUIEnvironment();

  //the settings dialog is not needed any more
  delete pSettings;

  device->setWindowCaption(L"irrOdeMarble - an irrOde demo game");

  //register the IrrOde scene node factory
  CIrrOdeSceneNodeFactory cFactory(smgr);
  smgr->registerSceneNodeFactory(&cFactory);

  while (device->getTimer()->isStopped()) device->getTimer()->start();

  //Now let's add menu, game and about screen
  pStates.push_back(new CMenu (device));
  pStates.push_back(new CPlay (device));
  pStates.push_back(new CAbout(device));
  pActive=pStates[0];

  //activate the first of the states .. the menu
  pActive->activate();

  iRet=0;
  int lastFPS=-1;

  device->run();
  //now run the device
  while(device->run() && !bQuit) {
    //update the active state
    iRet=pActive->update();

    //if the active state returned a different value than "0" the state needs to be changed
    if (iRet) {
      iActive=iRet-1;
      pActive->deactivate();

      //if the number returned from the active state is bigger than the number of states the application quits
      if (iActive<pStates.size()) {
        pActive=pStates[iActive];
        pActive->activate();
      }
      else bQuit=true;
    }

    //let's do the usual Irrlicht stuff...
    driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
    int fps = driver->getFPS();

    if (lastFPS != fps) {
      core::stringw str = L"irrOdeMarble - an irrOde demo game [";
      str += driver->getName();
      str += "] FPS:";
      str += fps;

      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
  }

  //now clean up the states
  for (u32 i=0; i<pStates.size(); i++) delete pStates[i];
  device->drop();

  return 0;
}


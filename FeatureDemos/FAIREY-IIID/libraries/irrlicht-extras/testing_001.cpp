#include <irrlicht.h>      //for graphics fun
#include "driverChoice.h"   //runtime select video drivers
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup") //disrupts driverChoiceConsole when enabled
#endif
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
scene::ICameraSceneNode* Camera[2] = {0,0};
IrrlichtDevice *Device = 0;
core::stringw MessageText = "MessageText";
core::stringw Caption = "CaptionText";

enum {
   GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000,
   GUI_ID_POSITION_TEXT,
   GUI_ID_ABOUT,
   GUI_ID_WINDOW,
   GUI_ID_QUIT,
};
void setActiveCamera(scene::ICameraSceneNode* newActive)
{
        if (0 == Device)
                return;

        scene::ICameraSceneNode * active = Device->getSceneManager()->getActiveCamera();
        active->setInputReceiverEnabled(false);

        newActive->setInputReceiverEnabled(true);
        Device->getSceneManager()->setActiveCamera(newActive);
}
void showAboutText() // create modal message box with the text
{
   Device->getGUIEnvironment()->addMessageBox( Caption.c_str(), MessageText.c_str() );
}
void showWindow() //create a window for the fun of it
{
   Device->getGUIEnvironment()->addWindow( core::rect<s32>(300,45,800,480),false,L"Character Window",0,GUI_ID_DIALOG_ROOT_WINDOW);
}


class MyEventReceiver : public IEventReceiver
{
public:
virtual bool OnEvent(const SEvent& event)
   {
      if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown == false)
      {
         if ( OnKeyUp(event.KeyInput.Key) ) return true;
      }
      if (event.EventType == EET_GUI_EVENT)
      {
         s32 id = event.GUIEvent.Caller->getID();
         IGUIEnvironment* env = Device->getGUIEnvironment();
         switch(event.GUIEvent.EventType)
         {
            case EGET_MENU_ITEM_SELECTED: // a menu item was clicked
             OnMenuItemSelected( (IGUIContextMenu*)event.GUIEvent.Caller );
             break;
            default:
             break;
         }
      }
      return false;
   }
bool OnKeyUp(irr::EKEY_CODE keyCode)
{
   if (keyCode == irr::KEY_ESCAPE)
   {
      if (Device)
      {
         scene::ICameraSceneNode * camera = Device->getSceneManager()->getActiveCamera();
         if (camera)
         {
            camera->setInputReceiverEnabled( !camera->isInputReceiverEnabled() );
         }
         return true;
      }
   }
return false;
}
void OnMenuItemSelected( IGUIContextMenu* menu )
{
s32 id = menu->getItemCommandId(menu->getSelectedItem());
IGUIEnvironment* env = Device->getGUIEnvironment();
switch(id)
{
   case GUI_ID_QUIT:
      Device->closeDevice();
      break;
   case GUI_ID_ABOUT:
      showAboutText();
      break;
   case GUI_ID_WINDOW:
      showWindow();
      break;
}
}
};
int main()
{
   //irrlicht event receiver
   MyEventReceiver receiver;

   //ask the user for the video driver to use
   video::E_DRIVER_TYPE driverType=driverChoiceConsole();
        if (driverType==video::EDT_COUNT) return 1;

   //irrlicht device
   Device = createDevice(driverType, dimension2d<u32>(800, 600), 16, false, false, false, &receiver);
        if (!Device) return 1;

   //irrlicht services
   IVideoDriver*      driver   = Device->getVideoDriver();
        ISceneManager*      smgr   = Device->getSceneManager();
        IGUIEnvironment*   env      = Device->getGUIEnvironment();

   //make our players camera
   smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

   //our top menu bar
   IGUIContextMenu* menu = env->addMenu();
        menu->addItem(L"File", -1, true, true);
        menu->addItem(L"Help", -1, true, true);
   IGUIContextMenu* submenu;
        submenu = menu->getSubMenu(0);
        submenu->addSeparator();
        submenu->addItem(L"Quit", GUI_ID_QUIT);
   submenu = menu->getSubMenu(1);
   submenu->addItem(L"TestWindow", GUI_ID_WINDOW);
        submenu->addItem(L"About", GUI_ID_ABOUT);

   //anchor our fpstext on the screen (we update it from inside the main loop)
   IGUIStaticText* fpstext = env->addStaticText(L"", core::rect<s32>(400,4,570,23), true, false, menu);

   // set window caption
        Caption += " - [";
        Caption += driver->getName();
        Caption += "]";
        Device->setWindowCaption(Caption.c_str());

   // add our camera scene nodes
        Camera[0] = smgr->addCameraSceneNodeMaya();
        Camera[0]->setFarValue(20000.f);
        // Maya cameras reposition themselves relative to their target, so target the location
        // where the mesh scene node is placed.
        Camera[0]->setTarget(core::vector3df(0,30,0));
   Camera[1] = smgr->addCameraSceneNodeFPS();
        Camera[1]->setFarValue(20000.f);
        Camera[1]->setPosition(core::vector3df(0,0,-70));
        Camera[1]->setTarget(core::vector3df(0,30,0));
   setActiveCamera(Camera[0]);

      while(Device->run()) //our main loop
      {
      driver->beginScene(true, true, SColor(255,100,101,140));
      smgr->drawAll();
      env->drawAll();
      driver->endScene();
      int fps = driver->getFPS();
      int lastFPS = -1;
      if (lastFPS != fps)
      {
         core::stringw tmp(L"[");
         tmp += driver->getName();
         tmp += L"] fps: ";
         tmp += fps;
         fpstext->setText(tmp.c_str());
         lastFPS = fps;
      }
      Device->yield();
   }
   Device->drop();
   return 0;
}

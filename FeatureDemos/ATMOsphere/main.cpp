
#include <stdio.h>
#include <math.h>
#include <wchar.h>
#include <irrlicht.h>
#include <IGUIFont.h>
#include "ATMOsphere.cpp"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

scene::ISceneNode* node = 0;
IrrlichtDevice* device = 0;
bool kursorius=false;
bool kamera=true;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera = 0;
scene::ICameraSceneNode* camera2 = 0;
ATMOsphere *atmo;
class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent& event)
   {
      /*
      If the key 'W' or 'S' was left up, we get the position of the scene node,
      and modify the Y coordinate a little bit. So if you press 'W', the node
      moves up, and if you press 'S' it moves down.*/
   if (event.EventType == irr::EET_KEY_INPUT_EVENT&&
         event.KeyInput.PressedDown)
      {
         switch(event.KeyInput.Key)
         {
            case KEY_ESCAPE:{device->closeDevice();}
         case KEY_SPACE:{
                 kursorius=!kursorius;
                 device->getCursorControl()->setVisible(kursorius);
                 kamera=!kamera;
                 core::vector3df targ;
                 if (kamera){
                 device->getCursorControl()->setPosition(0.5f,0.5f);
                 smgr->setActiveCamera(camera);
                 camera->setInputReceiverEnabled(true);
                 }
                 else {
                 camera->setInputReceiverEnabled(false);
                 targ=camera->getTarget();
                 camera2->setTarget(targ);
                 camera2->setPosition(camera->getPosition());
                 smgr->setActiveCamera(camera2);
                 }
                 }
         case KEY_KEY_S:
            {
            //   core::vector3df v = node->getPosition();
            //   v.Y += event.KeyInput.Key == KEY_KEY_W ? 2.0f : -2.0f;
            //   node->setPosition(v);
            }
            default:
            break;

            return true;
         }
      }

if (event.EventType == EET_GUI_EVENT)
      {
         s32 id = event.GUIEvent.Caller->getID();
         //IGUIEnvironment* env = device->getGUIEnvironment();

         switch(event.GUIEvent.EventType)
         {

         case EGET_SCROLL_BAR_CHANGED:
            if (id == 104)
            {
               s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
               atmo->setDaysPerDay((f64)pos/10.0f);//day speed

            }
            break;
            default:
            break;
    }return true;
    }


      return false;
   }
};


/*
The event receiver for moving a scene node is ready. So lets just create
an Irrlicht Device and the scene node we want to move. We also create some
other additional scene nodes, to show that there are also some different
possibilities to move and animate scene nodes.

*/


int main()
{

MyEventReceiver receiver;
   device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(800, 600),
      32,false, true, false, &receiver);//EDT_DIRECT3D9

   device->setEventReceiver(&receiver);
   video::IVideoDriver* driver = device->getVideoDriver();

      IGUIEnvironment* env = device->getGUIEnvironment();
   driver->setTextureCreationFlag(ETCF_ALWAYS_32_BIT,true);
   smgr = device->getSceneManager();

   IGUIScrollBar* scrollbar = env->addScrollBar(true, rect<s32>(150, 45, 350, 60), 0, 104);
   scrollbar->setMax(50000);
   scrollbar->setPos(600);
   IGUIStaticText *textinfo;
   textinfo=env->addStaticText(L"Dayspeed in minutes", rect<s32>(150,25,350,40), true);
   IGUIStaticText *textinfo3;
   textinfo3=env->addStaticText(L"Press spacebar for mouse cursor and again spacebar for mouse rotate.Arrows to move.", rect<s32>(50,5,550,20), true);
   IGUIStaticText *textinfo2;
   textinfo2=env->addStaticText(L"Dayspeed is how long virtual day takes in real day(1440 min) in minutes.\nExample: 2880(dayspeed) / const 1440(day in minutes) = 2 days per 1 minute", rect<s32>(50,70,550,100), true);
   /*
   To make the font a little bit nicer, we load an external font
   and set it as new font in the skin. An at last, we create a
   nice Irrlicht Engine logo in the top left corner.
   */
   IGUISkin* skin = env->getSkin();
   IGUIFont* font = env->getFont("../../media/fonthaettenschweiler.bmp");
   if (font)
      skin->setFont(font);
   camera = smgr->getActiveCamera();
   if (camera)
   {
      smgr->setActiveCamera(0);
      camera->remove();
   }
camera2 = smgr->addCameraSceneNode();
   camera2->setFOV(45.0f*180.0f/irr::core::DEGTORAD);//make correct FOV
   camera2->setFarValue(120000.0f);
   camera = smgr->addCameraSceneNodeFPS(0, 100, 500);
   camera->setFOV(45.0f*180.0f/irr::core::DEGTORAD);
   camera->setFarValue(120000.0f);
   camera->setPosition(core::vector3df(0,20,20));
//----------------------------------
    atmo=new ATMOsphere;
    atmo->start(device,driver,smgr->getRootSceneNode(),smgr,666);
 //int laikas=-1;

//u8 spalva=0;

//**************************
   scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
      "../../media/terrain-heightmap.bmp");

   terrain->setScale(core::vector3df(10, 1.0f, 10));
   terrain->setMaterialFlag(video::EMF_LIGHTING, true);
   terrain->setPosition(core::vector3df(-1200,-50,-1400));
   terrain->setMaterialType(terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
   video::EMT_DETAIL_MAP : video::EMT_SOLID);
   terrain->setMaterialTexture(0, driver->getTexture("../../media/terrain-texture.jpg"));
   terrain->setMaterialTexture(1, driver->getTexture("../../media/detailmap3.jpg"));
   //terrain->setMaterialType(video::EMT_DETAIL_MAP);
   terrain->scaleTexture(1.0f, 50.0f);

   // create triangle selector for the terrain
   scene::ITriangleSelector* selector
   = smgr->createTerrainTriangleSelector(terrain, 0);
   terrain->setTriangleSelector(selector);

   // create collision response animator and attach it to the camera
   scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
   selector, camera, core::vector3df(60,100,60),
   core::vector3df(0,0,0),
   core::vector3df(0,50,0));
   selector->drop();
   camera->addAnimator(anim);
   anim->drop();

   device->getCursorControl()->setVisible(false);
   int lastFPS = -1;

   while(device->run())
   {
    atmo->update(driver);//update all sky/sun
    driver->beginScene(true, true, video::SColor(255,113,113,133));
   smgr->drawAll();
   env->drawAll();
   driver->endScene();
    int fps = driver->getFPS();
    wchar_t tmp[1024];int fp;
   if (lastFPS != fps)
      { fp=fps;
        lastFPS = fps;
        }

         swprintf(tmp, 1024, L" [%s] fps: %d dayspeed: %5.3f",
            driver->getName(),fp,atmo->getDayspeed());


stringw str = "FPS= ";
str += fp;

font->draw(str.c_str(),rect<s32>(5,5,30,30),SColor(255,255,0,100));

            device->setWindowCaption(tmp);

   }

   device->drop();

   return 0;
}

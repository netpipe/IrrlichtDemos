
#include "GameClock.h"
#include "Reciever.h"
#include "Globals.h"

#define USE_3D_SCENE

ISceneNode* Skybox;

int main(int argc, char** argv)
{

    int ResX = 800, ResY = 600;

    device = createDevice(EDT_OPENGL, dimension2du(ResX, ResY), 16, false, false, false, 0);

   MyEventReceiver receiver;
   device->setEventReceiver(&receiver);

    device->setWindowCaption(L"GameClock Example");

    IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

   // set a font
 //  IGUISkin* skin = guienv->getSkin();
  // IGUIFont* font = guienv->getFont("fontModern.bmp");
  // if (font)
  //    skin->setFont(font);

#ifdef USE_3D_SCENE

    ISceneManager* smgr = device->getSceneManager();

   IAnimatedMesh* EarthMesh = smgr->getMesh("../../media/earth.x");

   //The Camera
//   smgr->addCameraSceneNodeFPS(0);
   irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0));
   camera->setTarget(vector3df(0,0,50));


   if (EarthMesh)
   {
      //create mesh copy with tangent informations from original earth.x mesh
      IMesh* tangentSphereMesh = smgr->getMeshManipulator()->createMeshWithTangents(EarthMesh->getMesh(0));

      //scale the mesh by factor 20
      smgr->getMeshManipulator()->scaleMesh(tangentSphereMesh, vector3df(13,13,13));

      //create mesh scene node
      ISceneNode* Sphere = smgr->addMeshSceneNode(tangentSphereMesh);
      Sphere->setPosition(vector3df(0,0,50));

/*        if(driverType == EDT_DIRECT3D9 || driverType == EDT_DIRECT3D8)
        {
            //load heightmap, create normal map from it and set it
            ITexture* EarthNormalMap = driver->getTexture("earthbump.bmp");
            driver->makeNormalMapTexture(EarthNormalMap, 20.0f);
            Sphere->setMaterialTexture(1, EarthNormalMap);
            Sphere->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
        }
*/
      //add rotation animator
      ISceneNodeAnimator* anim = smgr->createRotationAnimator(vector3df(0,0.10f,0));
      Sphere->addAnimator(anim);
      anim->drop();

      //drop mesh because we created it with a create.. call.
      tangentSphereMesh->drop();
   }

   //Light
   smgr->addLightSceneNode(0, vector3df(5,3,5),SColorf(0.5f,0.5f,0.80f,1.0f), 1000.0f);

   //Skybox
   Skybox = smgr->addSkyBoxSceneNode(
      driver->getTexture("stars3.jpg"),
      driver->getTexture("stars3.jpg"),
      driver->getTexture("stars3.jpg"),
      driver->getTexture("stars3.jpg"),
      driver->getTexture("stars3.jpg"),
      driver->getTexture("stars.jpg"));

//    Skybox = smgr->addSkyDomeSceneNode(driver->getTexture("stars.jpg"), 30, 30, 1, 2);

#endif

    for(s32 i=0; i<gui::EGDC_COUNT; ++i)
   {
        SColor col = device->getGUIEnvironment()->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
        col.setAlpha(255);
        device->getGUIEnvironment()->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
   }

   gui::IGUIToolBar* bar = guienv->addToolBar();
   gui::IGUIStaticText* fpstext = guienv->addStaticText(L"", core::rect<s32>(ResX-700,4,ResX-620,23), true, 0, bar);
   gui::IGUIStaticText* deltatext = guienv->addStaticText(L"", core::rect<s32>(ResX-800,4,ResX-700,23), true, true);
   gui::IGUIStaticText* speedtext = guienv->addStaticText(L"", core::rect<s32>(ResX-350,4,ResX-220,23), true, true);
   gui::IGUIStaticText* timetext = guienv->addStaticText(L"", core::rect<s32>(ResX-120,4,ResX-20,23), true, true);
//    timetext->setToolTipText(L"%i,", gameclock.getClockDays());

   guienv->addButton(rect<s32>(200, 5, 230, 25), 0, 101, L"<-");
   guienv->addButton(rect<s32>(240, 5, 290, 25), 0, 102, L"Pause");
   guienv->addButton(rect<s32>(300, 5, 340, 25), 0, 103, L"Play");
   guienv->addButton(rect<s32>(350, 5, 380, 25), 0, 104, L">");
   guienv->addButton(rect<s32>(390, 5, 420, 25), 0, 105, L">>");

    // initiate the timer class
    timer = device->getTimer();
    gameclock.init(timer);

    // you can't set the clock before 1:00am on the first day
    // I'm still thinking how I could change that


    gameclock.setClock( (device->getTimer()->getDayTimeHour()),(device->getTimer()->getDayTimeMin()),(device->getTimer()->getDayTimeSec()),"AM");

    while(device->run() && driver)
    {
      if (device->isWindowActive())
      {
//        printf("the time is now : %u hours \n" ,(device->getTimer()->getDayTimeS()/(60))     );
         driver->beginScene(true, true, video::SColor(0,10,50,130));

            #ifdef USE_3D_SCENE
         smgr->drawAll();
         #endif

         guienv->drawAll();

         driver->endScene();

         //switched to irrlicht sleep
            //device->sleep();

      }
      else
        {
            device->yield();
        }

        // update the timer after the window active statement to keep current
        gameclock.update();

        core::stringw str(L" FPS: ");
        str.append(core::stringw(driver->getFPS()));
        fpstext->setText(str.c_str());

        core::stringw str3(L" Delta: ");
        str3.append(core::stringw(gameclock.getDeltaTime()));
        deltatext->setText(str3.c_str());

        core::stringw str4(L" Speed: ");
        str4.append(core::stringw((int)gamespeed));
        speedtext->setText(str4.c_str());

        // the game clock
        timetext->setText(core::stringw(gameclock.getClockDisplay()).c_str());
    }

    device->drop();
    return 0;
}


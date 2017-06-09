//3ds, b3d A, X A, ms3d A, oct L and lmts L
/*=====================================
CREDITS
PROGRAMMER MATTHEW KIELAN, DEVUSH, DEVSH OR EDUARDO
REALISTIC WATER SCENE NODE by elvman

Thank you hybrid without you i would never do split screen
=======================================*/
#include "ScreenQuad.h"
#include </usr/include/GL/glew.h>
#include </usr/include/GL/gl.h>
#include </usr/include/GL/glut.h>



void SinglePlayerLoop(player* player1,RealisticWaterSceneNode* Water);
void SplitScreenLoop(player* player1,player* player2,RealisticWaterSceneNode* Water);
bool MenuLoop(RealisticWaterSceneNode* Water);
void OptionsLoop(irr::video::ITexture* NinjaLogo, RealisticWaterSceneNode* Water=NULL);

int main(int argc, char** argv) {
   loadmenu();
   std::cout << res.x << " x " << res.y <<std::endl;
   device = irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::s32>(res.x, res.y), 32, fullscreen, stencilbuffer, vsync_enabled, &EventRec);
   if (!device) {
      std::cout << "Error 3: Graphical display initialisation failed" << std::endl;
      return 1;
   }

   intro();
   device->getFileSystem()->addZipFileArchive("./Game Data/data20080511.pk3");
   irr::video::IVideoDriver* driver = device->getSceneManager()->getVideoDriver();
   irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
   irr::gui::IGUISkin* skin = guienv->getSkin();
   irr::gui::IGUIFont* font = guienv->getFont("./media/fonthaettenschweiler.bmp");
   if (driver->queryFeature(irr::video::EVDF_VERTEX_SHADER_2_0) && driver->queryFeature(irr::video::EVDF_PIXEL_SHADER_2_0)) DOF_enabled = false;
   if (font)
       skin->setFont(font);
   skin->setFont(guienv->getBuiltInFont(), irr::gui::EGDF_TOOLTIP);
   loadjoy();
   joystick1 = new joystick(jdf, Gamepad, rdf);
   if (!joystick1)
      joystick1 = new joystick("/dev/input/js0", Gamepad, "/dev/input/event6");


// MUSIC INITIALISATION BEGIN
   irrklang::ISound* music = enginesound->play2D(track[track_count], false, false, true);
   MusicEndedReceiver* musicstopped = new MusicEndedReceiver();
   if (music) {
      if (!loadmenu(music, musicstopped))
         music->setVolume(Music_Volume);
         if (musicstopped)
            music->setSoundStopEventReceiver(musicstopped);
   }
   else
      std::cout << "Error 2: Pointer music non-existent (maybe failure in initialisation)." << std::endl;
//MUSIC INITIALIZATION FINISHED*/
   change_volume(0.0);


   player* player1 = new player(irr::core::vector3df(0,0,0), device, true, "./media/sydney.md2");
   if (!init_player(player1))
      return 1;
   player* player2 = new player(irr::core::vector3df(700,-384,955), device, false, "./media/ninja.b3d");
   if (!init_player(player1, joystick1, player1))
      return 1;
   player2->getnode()->setVisible(false);
   RealisticWaterSceneNode* Water=new RealisticWaterSceneNode(1228,1640, driver->getTexture("./waterbump.png"),device->getTimer());

   while(device->run()) {
      irr::video::ITexture* NinjaLogo = driver->getTexture("./PNSLogo.png");
      guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.6,res.x/1.6,res.y/1.41), 0, GUI_ID_OPTIONS_BUTTON, L"Options", L"");
      guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/2.4,res.x/1.6,res.y/2), 0, GUI_ID_SINGLE_PLAYER_BUTTON, L"Single Player Game", L"");
      guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.92,res.x/1.6,res.y/1.66), 0, GUI_ID_SPLIT_SCREEN_BUTTON, L"Split Screen Game", L"");
      guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.2,res.x/1.6,res.y/1.09), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exit To Unix?");

      while(device->run()) {
         driver->beginScene(true, true, irr::video::SColor(255,0,0,0));
         driver->draw2DImage (NinjaLogo, irr::core::rect<irr::s32>(0,0,res.x,res.y), irr::core::rect<irr::s32>(0,0,1280,960),0,0,true);
         guienv->drawAll();
         driver->endScene();
         if (EventRec.IsButtonClicked(GUI_ID_SINGLE_PLAYER_BUTTON)) {
            guienv->clear();
            SinglePlayerLoop(player1, Water);
            break;
         }
         else if (EventRec.IsButtonClicked(GUI_ID_QUIT_BUTTON)) {
            guienv->clear();
            driver->removeAllTextures();
            savemenu();
            device->closeDevice();
            break;
         }
         else if (EventRec.IsButtonClicked(GUI_ID_OPTIONS_BUTTON)) {
            guienv->clear();
            OptionsLoop(NinjaLogo);
            guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.6,res.x/1.6,res.y/1.41), 0, GUI_ID_OPTIONS_BUTTON, L"Options", L"");
            guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/2.4,res.x/1.6,res.y/2), 0, GUI_ID_SINGLE_PLAYER_BUTTON, L"Single Player Game", L"");
            guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.92,res.x/1.6,res.y/1.66), 0, GUI_ID_SPLIT_SCREEN_BUTTON, L"Split Screen Game", L"");
            guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.2,res.x/1.6,res.y/1.09), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exit To Unix?");
         }
         else if (EventRec.IsButtonClicked(GUI_ID_SPLIT_SCREEN_BUTTON)) {
            guienv->clear();
            SplitScreenLoop(player1, player2, Water);
            break;
         }
      }
   }
   savemenu();
   /*MUSIC DEINITIALIZATION*/
   changing=true; // to fool the class - to make it not respond to the next line
   enginesound->removeSoundSource(track[track_count]); // clear the pointer
   delete musicstopped; // clear up the memory
   music->drop();
   enginesound->drop(); // delete engine
   return 0;
}

void SplitScreenLoop(player* player1,player* player2,RealisticWaterSceneNode* Water) {
   player2->getnode()->setVisible(true);
   device->getCursorControl()->setVisible(false);
   irr::video::IVideoDriver* driver = device->getSceneManager()->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();


   irr::scene::IAnimatedMesh* qmapmesh = smgr->getMesh("./Game Data/data20080511.pk3/maps/ruiner.bsp");
   irr::scene::ISceneNode* qmapnode = NULL;
   if (qmapmesh) {
      qmapmesh->setHardwareMappingHint(irr::scene::EHM_STATIC);
      qmapnode = smgr->addMeshSceneNode(qmapmesh->getMesh(0));
   }

   player2->getnode()->setFrameLoop(184, 205);
   player2->getnode()->setAnimationSpeed(8);
   player2->getnode()->setScale(irr::core::vector3df(12.0f));
   player2->getnode()->setVisible(true);


   Water->setPosition(irr::core::vector3df(154,-366,603));
   Water->setWaterColor(irr::video::SColor(0.1f, 0.1f, 0.6f, 1.0f));
   Water->setWindForce(10.0f);

   irr::scene::ICameraSceneNode* cam1= device->getSceneManager()->addCameraSceneNodeFPS();
   irr::scene::ICameraSceneNode* cam2= device->getSceneManager()->addCameraSceneNode(player2->getnode(), irr::core::vector3df(0,12,-10), irr::core::vector3df(player2->getnode()->getPosition().X,player2->getnode()->getPosition().Y,player2->getnode()->getPosition().Z+50));
   cam1->setPosition(cam2->getAbsolutePosition());
   cam1->setAspectRatio(4.0f/1.5f);
   cam2->setAspectRatio(4.0f/1.5f);


   ScreenQuad* QUAD = new ScreenQuad(cam1,cam2);
   QUAD->addNodeToDepthPass(qmapnode);
   QUAD->addNodeToDepthPass(player1->getnode());
   QUAD->addNodeToDepthPass(player2->getnode());


   int lastFPS = -1;
   int fps;
   while (device->run()) {
      if (device->isWindowActive()) {
         driver->beginScene(false, false);
         QUAD->renderSplit();/*
         Water->OnAnimate(device->getTimer()->getTime(),2);
         driver->setRenderTarget(QUAD->RT2, false, false);
         Water->render();*/
         smgr->drawAll();
         QUAD->renderSplit2();/*
         Water->OnAnimate(device->getTimer()->getTime(),1);
         driver->setRenderTarget(QUAD->RT, false, false);
         Water->render();*/
         smgr->drawAll();
         QUAD->render();
         driver->endScene();
         fps = driver->getFPS();
         if (lastFPS != fps) {
            irr::core::stringw str = L"Project Ninja Star - Nexuiz Map example FPS:";
            str += fps;
            device->setWindowCaption(str.c_str());
            lastFPS = fps;
         }
         if (EventRec.IsKeyDown(irr::KEY_ESCAPE)) {
            if (MenuLoop(Water))
               break;
            usleep(40000);
         }
         if (EventRec.IsKeyDown(irr::KEY_KEY_S)) {
            driver->writeImageToFile(driver->createImageFromData(irr::video::ECF_A8R8G8B8, irr::core::dimension2d<irr::s32>(res.x,res.y/2), QUAD->DM->lock()), "./DM.png");
            driver->writeImageToFile(driver->createImageFromData(irr::video::ECF_A8R8G8B8, irr::core::dimension2d<irr::s32>(res.x,res.y/2), QUAD->DM2->lock()), "./DM2.png");
            driver->writeImageToFile(driver->createImageFromData(irr::video::ECF_A8R8G8B8, irr::core::dimension2d<irr::s32>(res.x,res.y/2), QUAD->RT->lock()), "./RT.png");
            driver->writeImageToFile(driver->createImageFromData(irr::video::ECF_A8R8G8B8, irr::core::dimension2d<irr::s32>(res.x,res.y/2), QUAD->RT2->lock()), "./RT2.png");
            QUAD->RT->unlock();
            QUAD->DM->unlock();
            QUAD->DM2->unlock();
            QUAD->RT2->unlock();
         }
      }
      else
         device->yield();
   }
   delete QUAD;
   savejoy(player2->getJoystick());
   qmapnode->remove();
   cam2->remove();
   cam1->remove();
   player2->getnode()->setVisible(false);
   return;
}

void SinglePlayerLoop(player* player1,RealisticWaterSceneNode* Water) {
   device->getCursorControl()->setVisible(false);
   irr::video::IVideoDriver* driver = device->getSceneManager()->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();

   irr::scene::IMesh* qmapmesh = smgr->getMesh("./Game Data/data20080511.pk3/maps/ruiner.bsp")->getMesh(0);
   irr::scene::ISceneNode* qmapnode = NULL;
   if (qmapmesh) {
      qmapnode = smgr->addOctTreeSceneNode(qmapmesh,0,-1,1024);
   }

   Water->setPosition(irr::core::vector3df(154,-360,603));
   Water->setWaterColor(irr::video::SColor(0.1f, 0.1f, 0.6f, 1.0f));
   Water->setWindForce(10.0f);

   irr::scene::ICameraSceneNode* camera= device->getSceneManager()->addCameraSceneNodeFPS();
   ScreenQuad* QUAD = new ScreenQuad();
   QUAD->addNodeToDepthPass(qmapnode);
   QUAD->addNodeToDepthPass(player1->getnode());

   irr::video::ITexture* colorMap = driver->getTexture("./media/rockwall.bmp");
   irr::video::ITexture* normalMap = driver->getTexture("./media/rockwall_height.bmp");

   driver->makeNormalMapTexture(normalMap, 9.0f);

   irr::scene::IMesh* tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(smgr->addHillPlaneMesh("bump", irr::core::dimension2d<irr::f32>(1228,1640),irr::core::dimension2d<irr::u32>(1,1))->getMesh(0));
   smgr->getMeshManipulator()->makePlanarTextureMapping(tangentMesh, 0.01f);
   irr::scene::IMeshSceneNode* room = smgr->addMeshSceneNode(tangentMesh);
   room->setMaterialTexture(0, colorMap);
   room->setMaterialTexture(1, normalMap);
   room->setMaterialType(irr::video::EMT_PARALLAX_MAP_SOLID);
                // adjust height for parallax effect
   room->getMaterial(0).MaterialTypeParam = 0.04f;
   room->setPosition(irr::core::vector3df(154,-380,603));
   irr::scene::ILightSceneNode* light1 = smgr->addLightSceneNode(0, irr::core::vector3df(154,-340,1203), irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 1000.0f);
   tangentMesh->drop();

   int lastFPS = -1;
   int fps;
   driver->setViewPort(irr::core::rect<irr::s32>(0,0,res.x,res.y));
   smgr->setActiveCamera(camera);
   while (device->run()) {
      if (device->isWindowActive()) {
         driver->beginScene(false, false, irr::video::SColor(255,255,255,255));
         QUAD->renderB();
         Water->OnAnimate(device->getTimer()->getTime(),1);
         driver->setRenderTarget(QUAD->RT, false, false);
         Water->render();
         smgr->drawAll();
         QUAD->render();
         driver->endScene();
         fps = driver->getFPS();
         if (lastFPS != fps) {
            irr::core::stringw str = L"Project Ninja Star - Nexuiz Map example FPS:";
            str += fps;
            device->setWindowCaption(str.c_str());
            lastFPS = fps;
         }
         if (EventRec.IsKeyDown(irr::KEY_ESCAPE)) {
            if (MenuLoop(Water))
               break;
            usleep(40000);
         }
      }
      else
         device->yield();
   }
   delete QUAD;
   light1->remove();
   room->remove();
   qmapnode->remove();
   camera->remove();
   driver->removeTexture(normalMap);
   driver->removeTexture(colorMap);
   return;
}

void OptionsLoop(irr::video::ITexture* NinjaLogo, RealisticWaterSceneNode* Water) {
   return;
}

bool MenuLoop(RealisticWaterSceneNode* Water) {
   irr::video::IVideoDriver* driver = device->getSceneManager()->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();
   irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
   driver->setViewPort(irr::core::rect<irr::s32>(0,0,res.x,res.y));

   irr::scene::ICameraSceneNode* CurrentCamera=smgr->getActiveCamera(); //get current camera
   irr::scene::ICameraSceneNode* Camera=smgr->addCameraSceneNode();
   Camera->setPosition(CurrentCamera->getPosition());
   Camera->setTarget(CurrentCamera->getTarget());
   smgr->setActiveCamera(Camera); //set the reflection camera


   irr::video::ITexture* NinjaLogo = driver->getTexture("./PNSLogo.png");
   device->getCursorControl()->setVisible(true);
   guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.6,res.x/1.6,res.y/1.41), 0, GUI_ID_OPTIONS_BUTTON, L"Options", L"");
   guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.92,res.x/1.6,res.y/1.66), 0, GUI_ID_LOAD_GAME_BUTTON, L"Load Last CheckPoint", L"Screwed up?");
   guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/2.4,res.x/1.6,res.y/2), 0, GUI_ID_RESUME_BUTTON, L"Resume", L"Back into the game");
   guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.37,res.x/1.6,res.y/1.23), 0, GUI_ID_MAIN_MENU_BUTTON, L"Exit to Menu", L"");
   guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.2,res.x/1.6,res.y/1.09), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exit To Unix?");
   for (irr::u32 i=0; i<irr::gui::EGDC_COUNT ; ++i) {
      irr::video::SColor col = guienv->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
      col.setAlpha(100);      guienv->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);   }
   while(device->run()) {
      driver->beginScene(true, true, irr::video::SColor(255,0,0,0));
      Water->OnAnimate(device->getTimer()->getTime(),0);
      Water->render();
      smgr->drawAll();
      driver->draw2DImage(NinjaLogo, irr::core::rect<irr::s32>(0,0,res.x,res.y), irr::core::rect<irr::s32>(0,0,1280,960),0,0,true);
      guienv->drawAll();
      driver->endScene();
      if (EventRec.IsButtonClicked(GUI_ID_RESUME_BUTTON)) {
         device->getCursorControl()->setVisible(false);
         smgr->setActiveCamera(CurrentCamera);
         driver->removeTexture(NinjaLogo);
         Camera->remove();
         guienv->clear();
         return false;
      }
      if (EventRec.IsButtonClicked(GUI_ID_QUIT_BUTTON)) {
         savemenu();
         device->closeDevice();
         return false;
      }
      if (EventRec.IsButtonClicked(GUI_ID_MAIN_MENU_BUTTON)) {
         smgr->setActiveCamera(CurrentCamera);
         driver->removeTexture(NinjaLogo);
         Camera->remove();
         guienv->clear();
         return true;
      }
      if (EventRec.IsButtonClicked(GUI_ID_OPTIONS_BUTTON)) {
         guienv->clear();
         OptionsLoop(NinjaLogo, Water);
         guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.6,res.x/1.6,res.y/1.41), 0, GUI_ID_OPTIONS_BUTTON, L"Options", L"");
         guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.92,res.x/1.6,res.y/1.66), 0, GUI_ID_LOAD_GAME_BUTTON, L"Load Last CheckPoint", L"Screwed up?");
         guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/2.4,res.x/1.6,res.y/2), 0, GUI_ID_RESUME_BUTTON, L"Resume", L"Back into the game");
         guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.37,res.x/1.6,res.y/1.23), 0, GUI_ID_MAIN_MENU_BUTTON, L"Exit to Menu", L"");
         guienv->addButton(irr::core::rect<irr::s32>(res.x/2.66,res.y/1.2,res.x/1.6,res.y/1.09), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exit To Unix?");
         for (irr::u32 i=0; i<irr::gui::EGDC_COUNT ; ++i) {
            irr::video::SColor col = guienv->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
            col.setAlpha(100);            guienv->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);         }
      }
      if (EventRec.IsButtonClicked(GUI_ID_LOAD_GAME_BUTTON)) {
         smgr->setActiveCamera(CurrentCamera);
         driver->removeTexture(NinjaLogo);
         guienv->clear();
         Camera->remove();
         loadgame();
         return false;
      }
      if (EventRec.IsKeyDown(irr::KEY_ESCAPE)) {
         device->getCursorControl()->setVisible(false);
         smgr->setActiveCamera(CurrentCamera);
         driver->removeTexture(NinjaLogo);
         guienv->clear();
         Camera->remove();
         return false;
      }
   }
}

#include <irrlicht.h>
#include <iostream>


using namespace irr;

int main()
{
        // create device and exit if creation failed

IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
      32, false, false, false);

        if (device == 0)
                return 1; // could not create selected driver.
////////////////////////////////////////////////////////////////////////////////////

        video::IVideoDriver* driver = device->getVideoDriver();
        scene::ISceneManager* smgr = device->getSceneManager();




        scene::IAnimatedMesh* TERRAIN = smgr->getMesh("../../media/room.3ds");

        scene::ISceneNode* node = 0;

        if (TERRAIN)
                node = smgr->addOctreeSceneNode(TERRAIN->getMesh(0), 0, 0, 0);
        if (node)
                node->setPosition(core::vector3df(0,0,0));

node->setMaterialFlag(video::EMF_FOG_ENABLE, true);

TERRAIN->setMaterialFlag(video::EMF_LIGHTING, true);
TERRAIN->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
TERRAIN->setMaterialFlag(video::EMF_FOG_ENABLE, true);


driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_LINEAR, 0.1f, 255.0f, 1.0f, false, true);



   smgr->addSkyBoxSceneNode(
      driver->getTexture("../../media/irrlicht2_up.jpg"),
      driver->getTexture("../../media/irrlicht2_dn.jpg"),
      driver->getTexture("../../media/irrlicht2_lf.jpg"),
      driver->getTexture("../../media/irrlicht2_rt.jpg"),
      driver->getTexture("../../media/irrlicht2_ft.jpg"),
      driver->getTexture("../../media/irrlicht2_bk.jpg"));

   scene::ILightSceneNode* light1 =
      smgr->addLightSceneNode(0, core::vector3df(100,100,0),
      video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 800.0f);

smgr->addCameraSceneNodeFPS();
device->getCursorControl()->setVisible(false);

        int lastFPS = -1;

        while(device->run())
        {
                if (device->isWindowActive())
                {
                        driver->beginScene(true, true, video::SColor(255,255,255,255));
                        smgr->drawAll();
                        driver->endScene();

                        int fps = driver->getFPS();

                        if (lastFPS != fps)
                        {
                                core::stringw str = L"Irr_project [";
                                str += driver->getName();
                                str += "] FPS:";
                                str += fps;

                                device->setWindowCaption(str.c_str());
                                lastFPS = fps;
                        }
                }
                else
                        device->yield();
        }

        device->drop();
        return 0;
}

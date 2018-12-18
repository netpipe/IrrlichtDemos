#include <irrlicht.h>
#include <iostream>
#include <stdio.h>
#include <wchar.h>

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

ICameraSceneNode *camera[3] = {0,0,0};

//Eventreceiver
class MyEventReceiver : public IEventReceiver {
public:
   virtual bool OnEvent(const SEvent& event){

   //Send all other events to cameras
   if (camera[1]) {
      camera[1]->OnEvent(event);
   }
    return false;
  }
};


int main()
{
    // ask user for driver

        video::E_DRIVER_TYPE driverType;

        printf("Please select the driver you want for this example:\n"\
                " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
                " (d) Software Renderer\n (e) Burning's Software Renderer\n"\
                " (f) NullDevice\n (otherKey) exit\n\n");

    //    char i;
   //     std::cin >> i;


      //Instance of the EventReceiver
      MyEventReceiver receiver;
      // create device and exit if creation failed
        IrrlichtDevice *device =
                createDevice(EDT_OPENGL, core::dimension2du(1056, 778),32,false,false,false,&receiver);
      if (device == 0) {
                return 1; // could not create selected driver.
      }
      IVideoDriver* driver = device->getVideoDriver();
      ISceneManager* smgr = device->getSceneManager();

      //load map
      device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
      IAnimatedMesh* map = smgr->getMesh("../../media/20kdm2.bsp");
        ISceneNode* map_node = 0;

      if (map) {
         map_node = smgr->addOctreeSceneNode(map->getMesh(0), 0, -1, 1024);
      }
      //set position of the map
      if (map_node) {
         map_node->setPosition(core::vector3df(-1300,-144,-1249));
      }

      //selector
      scene::ITriangleSelector* selector = 0;

        if (map_node)
        {
                map_node->setPosition(core::vector3df(-1350,-130,-1400));

                selector = smgr->createOctTreeTriangleSelector(
                                map->getMesh(0), map_node, 128);
                map_node->setTriangleSelector(selector);
        }



      //create cameras

      camera[0] = smgr->addCameraSceneNode();
      camera[0]->setFOV(PI/2);
      camera[0]->setAspectRatio(1.0f/1.0f);
      camera[0]->bindTargetAndRotation(true);

      camera[1] = smgr->addCameraSceneNodeFPS();
      camera[1]->setFOV(PI/2);
      camera[1]->setAspectRatio(1.0f/1.0f);

      camera[2] = smgr->addCameraSceneNode();
      camera[2]->setFOV(PI/2);
      camera[2]->setAspectRatio(1.0f/1.0f);
      camera[2]->bindTargetAndRotation(true);

      camera[1]->addChild(camera[0]);
      camera[1]->addChild(camera[2]);

   //disable cursoor
      device->getCursorControl()->setVisible(false);

      int lastFPS = -1;
        while(device->run())
        {
           //Set the viewpoint the the whole screen and begin scene
            driver->setViewPort(rect<s32>(0,0,1056,778));
            driver->beginScene(true,true,SColor(0,100,100,100));

            //define central viewport
            core::rect<s32> rect;
            rect.UpperLeftCorner.X = 355;
            rect.UpperLeftCorner.Y = 0;
            rect.LowerRightCorner.X = 702;
            rect.LowerRightCorner.Y = 768;

            vector3df camera1_pos = camera[1]->getAbsolutePosition();
            smgr->setActiveCamera(camera[1]);
            driver->setViewPort(rect);
                smgr->drawAll();

            //get central camera parameters
            vector3df camera_central_pos = camera[1]->getAbsolutePosition();
            vector3df camera_central_target = camera[1]->getTarget();
            vector3df camera_central_rotation = camera[1]->getRotation();


            //define left viewport
            rect.UpperLeftCorner.X = 0;
            rect.UpperLeftCorner.Y = 0;
            rect.LowerRightCorner.X = 348;
            rect.LowerRightCorner.Y = 768;

            //set camera parameters
            vector3df camera0_rotation;
            camera0_rotation.X = camera_central_rotation.X;
            camera0_rotation.Y = camera_central_rotation.Y-90;
            camera0_rotation.Z = camera_central_rotation.Z;
            camera[0]->setRotation(camera0_rotation);
            //end of setting camera parameter

            smgr->setActiveCamera(camera[0]);
            driver->setViewPort(rect);
            smgr->drawAll();

            //define right viewport
            rect.UpperLeftCorner.X = 708;
            rect.UpperLeftCorner.Y = 0;
            rect.LowerRightCorner.X = 1056;
            rect.LowerRightCorner.Y = 768;

            //set camera parameters
            vector3df camera2_rotation;
            camera2_rotation.X = camera_central_rotation.X;
            camera2_rotation.Y = camera_central_rotation.Y+90;
            camera2_rotation.Z = camera_central_rotation.Z;
            camera[2]->setRotation(camera2_rotation);
            //end of setting camera parameter

            smgr->setActiveCamera(camera[2]);
            driver->setViewPort(rect);
                smgr->drawAll();
                driver->endScene();

                int fps = driver->getFPS();

                if (lastFPS != fps)
                {
               core::stringw str = L"Quake 3 Map [";
                    str += driver->getName();
                    str += "] FPS:";
                    str += fps;

                    device->setWindowCaption(str.c_str());
                    lastFPS = fps;
                }

      }
      device->drop();
        return 0;
}

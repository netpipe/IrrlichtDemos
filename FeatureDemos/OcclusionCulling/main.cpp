
#include "Renderer.h"

int main()
{
  IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2d<s32>(800,600), 24, false);
  if (!device) return 0;

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr  = device->getSceneManager();

  Renderer  RenderMan(device, SColor(0, 10,10,10));
            RenderMan.addSky("data/models/occlusion/gradient3.bmp");
            RenderMan.addLight();
            //a.addWater("042eau.jpg");
            //a.addTerrain();
            RenderMan.loadModel("data/models/occlusion/etage.b3d", vector3df(0,0,0));
            RenderMan.addCam(.1f, vector3df(40,70,40), vector3df(0,0,1000), true);
            //a.addMy3DScene("models/bedroom1/bedroom.my3d", 0,0,0);
            RenderMan.initGUI();

  u32 then=0;
  while(device->run())
  {
      if (!device->isWindowActive())
                device->sleep(200);
      driver->beginScene(true, true, 0);
      smgr->drawAll();
      RenderMan.drawGUI();
      driver->endScene();

      if (then < device->getTimer()->getTime())
      {
         RenderMan.updateCulling();
         then = device->getTimer()->getTime() + 500;
      }
      device->sleep(1);
   }
}

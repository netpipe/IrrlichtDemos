#include <irrlicht.h>
#include <iostream>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;

#include "grass.h"



int main() {
IrrlichtDevice *device= createDevice(EDT_OPENGL, dimension2d<u32>(1024, 768), 32, false, true, false);
ISceneManager *smgr= device->getSceneManager();
IVideoDriver* video= device->getVideoDriver();
IGUIEnvironment* gui= device->getGUIEnvironment();

ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 40, 0.1, -1);
camera->setNearValue(0.1);
camera->setFarValue(4000);
device->getCursorControl()->setVisible(false);


ITerrainSceneNode* terrain= smgr->addTerrainSceneNode("media/level.bmp");
terrain->setScale(vector3df(1,0.4,1));
terrain->setMaterialTexture(0, video->getTexture("media/grass.jpg"));
terrain->setMaterialTexture(1, video->getTexture("media/detailmap.jpg"));
terrain->setMaterialType(EMT_DETAIL_MAP);
terrain->scaleTexture(10);

ILightSceneNode* light= smgr->addLightSceneNode(0, vector3df(150,250,150), SColorf(1,0.9,0.8), 1000);

grass grass;
grass.setSmgr(smgr);
grass.setParametres(150, 4.5, 5);
grass.setTerrain(terrain);
grass.setTexturesPerPlane(3);
grass.setAnisoFiltering(true);
grass.setLighting(false);

//for more details on this check documentation
//white with few greens level
grass.addGrassPlaneTemplate(video->getTexture("media/grass.png"), 3, 0.75, -99, 30);
grass.addGrassPlaneTemplate(video->getTexture("media/grass2.png"), 100, 0.75, -99, 30);
//blending level
grass.addGrassPlaneTemplate(video->getTexture("media/grass.png"), 50, 0.75, 30, 45);
grass.addGrassPlaneTemplate(video->getTexture("media/grass2.png"), 100, 0.75, 30, 45);
//green with a few whites level
grass.addGrassPlaneTemplate(video->getTexture("media/grass2.png"), 3, 0.75, 45, 60);
grass.addGrassPlaneTemplate(video->getTexture("media/grass.png"), 100, 0.75, 45, 60);
//add a no grass zone
aabbox3d<f32> noGrassHerePlease= aabbox3d<f32>(vector3df(150,0,100), vector3df(240, 100, 150));
grass.addNoGrassZone(noGrassHerePlease);

grass.makeReady();

camera->setPosition(vector3df(80,70,50));
camera->setTarget(terrain->getTransformedBoundingBox().getCenter());

while(device->run() && device) {

  grass.doGrassStuff(camera->getPosition());

  stringw str;
  str+= "FPS ";
  str+= video->getFPS();
  str+= "\n";

  str+= "Primitives ";
  str+= video->getPrimitiveCountDrawn();
  str+= "\n\n";

  str+= "Grasses: ";
  str+= grass.getCount();
  str+= "\n";

  IGUIStaticText* fps= gui->addStaticText(str.c_str(), rect<s32>(0,0,1000,1000));


  video->beginScene(true, true, SColor(255,0,0,255));
  smgr->drawAll();
  gui->drawAll();
  video->endScene();

  fps->remove();
  }
device->drop();
return 0;
}

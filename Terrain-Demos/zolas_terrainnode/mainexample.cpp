// see original faceofmars.cpp for credits :)
#include <irrlicht.h>
#include "CLMTerrainSceneNode.h"

#pragma comment(lib, "Irrlicht.lib")

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

int main(int argc,char *argv[])
{
   int cameraoffset=120;

   IrrlichtDevice *device =
   createDevice(video::EDT_OPENGL, core::dimension2d<s32>(800, 600),32,false);

   ISceneManager* smgr = device->getSceneManager();
   IVideoDriver* driver= device->getVideoDriver();
   IGUIEnvironment* env= device->getGUIEnvironment();

   IGUIStaticText* txt=env->addStaticText(L"camera()", rect<s32>(0,0,350,40), true);
   txt->setOverrideColor(SColor(255,0,0,0));
   txt->enableOverrideColor(true);

    IImage* heightimage = driver->createImageFromFile("terrain.bmp");
   IImage* skyimage = driver->createImageFromFile("sky.jpg");
   ITexture* terraintexture = driver->getTexture("terr_tex.jpg");
   ITexture* skytexture = driver->getTexture("sky_tex.jpg");

   CLMTerrainSceneNode* terrain = new CLMTerrainSceneNode(smgr->getRootSceneNode(), smgr, 9999);
   terrain->create(heightimage,terraintexture,0,32.0f,10.0f,1000.0f);
   terrain->getMaterial(0).FogEnable=true;
   terrain->getMaterial(0).Lighting=false;
   terrain->setAutomaticCulling(EAC_OFF);
   terrain->setPosition(vector3df(100,100,0));
   terrain->setRotation(vector3df(0,37,0));
   terrain->setDebugDataVisible(true);
   terrain->drop();


   scene::ICameraSceneNode* camera= smgr->addCameraSceneNodeFPS(0, 100, .10);
   camera->setFarValue(120000.0f);

   camera->setPosition(vector3df(0.0f,120.0f,0.0f));
   camera->setTarget(vector3df(0.0f,120.0f,-10.0f));
   camera->setUpVector(vector3df(0.0f,1.0f,0.0f));

   int lastFPS=-1;

   f32 fogDensity=0.0005f;
   SColor backgroundColor=irr::video::SColor(255,155,155,155);
   SColor fogColor=irr::video::SColor(0,155,155,155);
   wchar_t tmp[1024];
   matrix4 IDENTITY;
   IDENTITY.makeIdentity();
   SMaterial mat;
   mat.Lighting=false;
   matrix4 invTrans;
   matrix4 Trans;
   while(device->run())
   {

      vector3df pos = camera->getPosition();
      f32 nearp = camera->getNearValue();
      f32 farp=camera->getFarValue();
      Trans=terrain->getAbsoluteTransformation();
      Trans.getInverse(invTrans);

      invTrans.transformVect(pos);
      float glide = terrain->getHeight(pos.X,pos.Z)+30.0f*nearp;
      if (glide<0.0f) glide = 0.0f;
      pos.Y=pos.Y>glide? pos.Y:glide;

      Trans.transformVect(pos);

      camera->setPosition(vector3df(pos.X,pos.Y,pos.Z));
      //camera->setTarget(tgt);

      driver->setFog(fogColor, false, 10.0f, 2000.0f, fogDensity, false, false);
      swprintf(tmp,1024,L"camera( %.3f,  %.3f,  %.3f )",pos.X,pos.Y,pos.Z);
      txt->setText(tmp);

      driver->beginScene(true, true, backgroundColor);
      smgr->drawAll();

      driver->setTransform(video::ETS_WORLD, IDENTITY);
      driver->setMaterial(mat);
      driver->draw3DLine(vector3df(0,200,0),vector3df(farp,0,0),SColor(255,255,0,0));
      driver->draw3DLine(vector3df(0,200,0),vector3df(0,farp,0),SColor(255,0,255,0));
      driver->draw3DLine(vector3df(0,200,0),vector3df(0,0,farp),SColor(255,0,0,255));

      env->drawAll();
      driver->endScene();

      int fps = driver->getFPS();
      if (lastFPS != fps)
      {
         swprintf(tmp, 1024, L"Terrainengine-libmini and Irrlicht Engine (fps:%d) Triangles:%d",
            fps, driver->getPrimitiveCountDrawn());
         device->setWindowCaption(tmp);
         lastFPS = fps;
      }


   }

   device->drop();

   return 0;
}

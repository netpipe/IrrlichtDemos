#ifndef BXWRAPPERH
#define BXWRAPPERH

#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

#include "OcclusionCulling.h"

class Renderer
{
  IrrlichtDevice* device;
  IGUIEnvironment* guienv;
  IVideoDriver* driver;
  ISceneManager* smgr;
  scene::IMetaTriangleSelector* META;
  ITerrainSceneNode* terrain;
  ICameraSceneNode* cam;
  IGUIStaticText* texte;
  SColor Diffuse;
  OcclusionCulling* cull; //load map and occlusion culling

public:
 Renderer(IrrlichtDevice* devic, SColor diffuse);
 void addCam(f32 speed, vector3df pos, vector3df tgt, bool gravity=false);
 void addLight();
 void addTerrain();
 void addWater(char* texture);
 void addSky(char* skytex);
 void addMy3DScene(char* scene, f32 X, f32 Y, f32 Z);
 void loadModel(char *modelfile, vector3df pos);
 void loadAnimatedModel(char *modelfile, vector3df pos);
 void loadFirstPersonWeaponModel(char *modelfile);
 void apply_light(SMaterial* mat);
 void initGUI();
 void drawGUI();
 void updateCulling();
 ~Renderer();
};

#endif

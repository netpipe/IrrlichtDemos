#ifndef __VIEWPRT_H__
#define __VIEWPRT_H__
// This class have all the tool to draw and use viewports (Perspective, Top, Front, Side)
//
//
#include "irrlicht.h"
#include "CGridSceneNode.h"

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;
using namespace scene;

class viewprt
{
public:
  IrrlichtDevice *device;
  scene::ICameraSceneNode* Camera[5];
  scene::IAnimatedMeshSceneNode* Model;
  scene::IBillboardSceneNode * bill;       // Origin reprensentation -- Billboard
  scene::ISceneNode* SkyBox;
  ISceneNode* ground;
  CGridSceneNode* grid;
  core::vector3df oldcampos[5];
  vector3df oldrot;
  bool perspalternate;                                          //button is pressed and not released
  bool moveitem;
  bool rotateitem;
  bool scaleitem;
  bool stretchitem;
  bool moveviews;             // Move the orthogonal views with a button
  bool viewports; // Default mode FALSE=Perspective view, TRUE=4 viewports or single views (ortho)
  bool lockviewport;
  bool singleview;
  bool gridvisibility;
  bool groundvisibility;
  bool gridvispers;
  bool validmove;
  bool validrotation;
  bool setOffset;
  bool loading;
  float viewscale2;
  float viewscale3;
  float viewscale4;
  float perspscale;
  float camX;
  float camY;
  float camZ;
  int viewrightspace;
  int currentview;



 viewprt ( bool perspalternate = false, bool moveitem = false, bool rotateitem = false, bool scaleitem = false,
          bool stretchitem = false, bool moveviews=false, bool viewports=false, bool lockviewport = false,
          bool singleview = true)
          {
             IrrlichtDevice *device = 0;
             scene::ICameraSceneNode* Camera[5]={0,0};
             scene::IAnimatedMeshSceneNode* Model = 0;
             core::vector3df oldcampos[5];
             oldcampos[5] = core::vector3df(0,0,0);

             scene::IBillboardSceneNode * bill=0;       // Origin reprensentation -- Billboard
             scene::ISceneNode* SkyBox=0;
             ISceneNode* ground = 0;
             CGridSceneNode* grid = 0;
             vector3df oldrot = vector3df(0,0,0);
             int viewrightspace=180;
             bool gridvisibility=true;
             bool groundvisibility=false;
             bool gridvispers=true;
             bool validmove=false;
             bool validrotation=false;
             bool setOffset=false;
             bool loading=false;
                                                    //button is pressed and not released

             float viewscale2=5.0f;
             float viewscale3=5.0f;
             float viewscale4=5.0f;
             float perspscale=0.0f;
             float camX=0;
             float camY=0;
             float camZ=0;

             int currentview=0;
               }

 bool checkForWindow(s32 element, s32 x, s32 y);
 void init(irr::IrrlichtDevice *Dev,int vrs);
 void setActiveCamera ( scene::ICameraSceneNode* newActive );
 void createCameras();
 void setModel(scene::IAnimatedMeshSceneNode* Mod);
 void setRightSpace(int rs);
 void resetViews(bool kind);
 int tellViewport(float mouseX, float mouseY);
 void moveViewport(int viewport, float mouseX, float mouseY, bool movetype=false);
 void drawGrid(int view, bool flag);
 void drawPerspectiveView();
 void drawTopView();
 void drawFrontView();
 void drawSideView();
 void drawSingle(int view);
 void drawViewPorts(float scale2, float scale3, float scale4, float scalepers);
 void createSkyBox();
 void createGround();
 void createGrid();
 void createBillboard();



};


#endif // __VIEWPRT_H__





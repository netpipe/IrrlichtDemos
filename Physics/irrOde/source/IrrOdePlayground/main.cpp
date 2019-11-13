#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <irrlicht.h>
#include <IrrODE.h>
#include <CIrrOdeDevice.h>

#include "myEventReceiver.h"
#include "myEventListeners.h"

#define _TERRAIN_HI "../../data/terrain-heightmap.bmp"
#define _TERRAIN_LO "../../data/terrain-heightmap_lo.bmp"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace ode;

CIrrOdeSurfaceParameters g_pParams[2];
int g_iParamNum=0;
/**
 * Each IrrOde scenenode gets it's unique ID from this function
 * @return the next scene node ID
 */
s32 getNextId() {
  static s32 id=0;
  return ++id;
}

/**
 * Add a new body to the scene
 * @param smgr pointer to the scene manager
 * @param driver pointer to the video driver
 * @param parent the parent node
 * @param pos position of the new node
 * @param rot rotation of the new node
 * @param scale scale of the new node
 * @param iType type of the new node (IrrOde node types only)
 * @param initPhysics should the "initPhysics" call be done here or later?
 * @return pointer to the new CIrrOdeBody object
 */
CIrrOdeBody *addNewBody(ISceneManager *smgr, IVideoDriver *driver, CIrrOdeSceneNode *parent, vector3df pos, vector3df rot, vector3df scale, ESCENE_NODE_TYPE iType, bool initPhysics=true) {
  ISceneNode *pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),parent);

  CIrrOdeBody *pNewBody=reinterpret_cast<CIrrOdeBody *>(pDummy);

  pNewBody->setID(getNextId());
  pNewBody->setPosition(pos);
  pNewBody->setRotation(rot);

  CIrrOdeGeom *pGeom=NULL;
  IAnimatedMesh *pMesh=NULL;
  IAnimatedMeshSceneNode *pNode=NULL;
  IAnimatedMeshSceneNode *pChildNode=NULL;
  IAnimatedMesh *pChildMesh=NULL;

  switch(iType) {
    //add a new box
    case IRR_ODE_GEOM_BOX_ID:
      pMesh=smgr->getMesh("../../data/box.3ds");
      pNode=smgr->addAnimatedMeshSceneNode(pMesh,pNewBody);
      pNode->setMaterialTexture(0,driver->getTexture("../../data/box0.jpg"));
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),pNode);
      break;

    //add a new capsule
    case IRR_ODE_GEOM_CAPSULE_ID:
      pMesh=smgr->getMesh("../../data/capsule.3ds");
      pNode=smgr->addAnimatedMeshSceneNode(pMesh,pNewBody);
      pNode->setMaterialTexture(0,driver->getTexture("../../data/capsule0.jpg"));
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_CAPSULE_NAME),pNode);
      break;

    //add a new cylinder
    case IRR_ODE_GEOM_CYLINDER_ID:
      pMesh=smgr->getMesh("../../data/cylinder.3ds");
      pNode=smgr->addAnimatedMeshSceneNode(pMesh,pNewBody);
      pNode->setMaterialTexture(0,driver->getTexture("../../data/cylinder0.jpg"));
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_CYLINDER_NAME),pNode);
      break;

    //add a new sphere
    case IRR_ODE_GEOM_SPHERE_ID:
      pMesh=smgr->getMesh("../../data/sphere.3ds");
      pNode=smgr->addAnimatedMeshSceneNode(pMesh,pNewBody);
      pNode->setMaterialTexture(0,driver->getTexture("../../data/sphere0.jpg"));
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_SPHERE_NAME),pNode);
      break;

    //add a new trimesh
    case IRR_ODE_GEOM_TRIMESH_ID:
      pMesh=smgr->getMesh("../../data/trimesh.3ds");
      pNode=smgr->addAnimatedMeshSceneNode(pMesh,pNewBody);

      pNode->setMaterialTexture(0,driver->getTexture("../../data/trimesh0.jpg"));

      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_TRIMESH_NAME),pNode);

      //the trimesh gets a child for collision detection
      pChildMesh=smgr->getMesh("../../data/trimesh_collision.3ds");
      pChildNode=smgr->addAnimatedMeshSceneNode(pChildMesh,pGeom);

      //the collision detection child is to be drawn in wireframe
      if (pChildNode) {
        pChildNode->setMaterialFlag(EMF_LIGHTING,false);
        pChildNode->setMaterialFlag(EMF_WIREFRAME,true);
      }
      break;

    //we don't add anything non-Irr-ODE here!
    default:
      printf("unsupported node type\n");
      return NULL;
  }

  //new node created? Set material attributes
  if (pNode) {
    pNode->setMaterialFlag(EMF_LIGHTING,true);
    pNode->setMD2Animation(scene::EMAT_STAND);
    pNode->setScale(scale);
    pNode->addShadowVolumeSceneNode();
  }

  //set the geom's mass and drop it
  pGeom=reinterpret_cast<CIrrOdeGeom *>(pDummy);
  pGeom->setMassTotal(scale.getLength()/64);
  pGeom->setSurfaceParameters(&g_pParams[++g_iParamNum%2],0);
  pGeom->drop();

  //init the body's physics and return it
  if (initPhysics) pNewBody->initPhysics();
  return pNewBody;
}

/**
 * This method creates a new joint, either connecting two objects or connecting one object to the world
 * @param smgr the scene manager
 * @param driver the video driver
 * @param parent the parent node
 * @param pos the position
 * @param rot the rotation
 * @param iType type of the joint. Use IRR_ODE_JOINT_*_ID only.
 * @param bLinkWorld link one object to the world (true) or two objects to each other (false)
 * @param bMotor add a motor to the link. The "Fixed" and "Ball and Socket" joints do not have any motors
 */
void addNewJoint(ISceneManager *smgr, IVideoDriver *driver, CIrrOdeSceneNode *parent, vector3df pos, vector3df rot, ESCENE_NODE_TYPE iType, bool bLinkWorld, bool bMotor) {
  CIrrOdeBody *pBody1=addNewBody(smgr,driver,parent,pos+vector3df(0,25,0),vector3df(0,0,0),vector3df(15,15,15),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID,false);
  vector3df thePos=vector3df(0,bLinkWorld?70:-35,0);
  CIrrOdeJoint *pJoint=NULL;
  ISceneNode *pDummy;
  vector3df dir=vector3df(1,0,0),axis=vector3df(1,0,0);

  switch(iType) {
    //add a fixed joint
    case IRR_ODE_JOINT_FIXED_ID:
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_FIXED_NAME),pBody1);
      pJoint=reinterpret_cast<CIrrOdeJoint *>(pDummy);
      pJoint->setPosition(thePos);
      break;

    //add a ball-and-socket joint
    case IRR_ODE_JOINT_BALL_SOCKET_ID:
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_BALL_SOCKET_NAME),pBody1);
      pJoint=reinterpret_cast<CIrrOdeJoint *>(pDummy);
      pJoint->setPosition(thePos);
      break;

    //add a hinge joint
    case IRR_ODE_JOINT_HINGE_ID:
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_HINGE_NAME),pBody1);
      pJoint=reinterpret_cast<CIrrOdeJoint *>(pDummy);
      pJoint->setPosition(thePos);

      //the hinge joint needs an axis. We use the camera's direction for that
      axis=rot.rotationToDirection(dir);
      ((CIrrOdeJointHinge *)pJoint)->setHingeAxis(axis);

      //if the joint has no motor we limit it, otherwise we initialize the motor
      if (!bMotor) {
        pJoint->setParameter(0,CIrrOdeJoint::eHiStop, M_PI/2);
        pJoint->setParameter(0,CIrrOdeJoint::eLoStop,-M_PI/2);
        pJoint->setParameter(0,CIrrOdeJoint::eBounce,1.0f);
      }
      else {
        CIrrOdeMotor *pMotor=reinterpret_cast<CIrrOdeMotor *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_MOTOR_NAME),pJoint));
        pMotor->setVelocity(1.0f);
        pMotor->setForce(500.0f);
        pMotor->setEnabled(true);
        pMotor->drop();
      }
      break;

    //add a new slider joint
    case IRR_ODE_JOINT_SLIDER_ID:
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_SLIDER_NAME),pBody1);
      pJoint=reinterpret_cast<CIrrOdeJoint *>(pDummy);
      pJoint->setPosition(thePos);

      //let's limit the joint
      pJoint->setParameter(0,CIrrOdeJoint::eHiStop, 25.0f);
      pJoint->setParameter(0,CIrrOdeJoint::eLoStop,-30.0f);
      pJoint->setParameter(0,CIrrOdeJoint::eBounce,  0.9f);

      //init the motor (if wanted)
      if (bMotor) {
        CIrrOdeMotor *pMotor=reinterpret_cast<CIrrOdeMotor *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_MOTOR_NAME),pJoint));
        pMotor->setVelocity(-3.0f);
        pMotor->setForce(500.0f);
        pMotor->setEnabled(true);
        pMotor->drop();
      }
      break;

    //add a new piston joint
    case IRR_ODE_JOINT_PISTON_ID:
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_PISTON_NAME),pBody1);
      pJoint=reinterpret_cast<CIrrOdeJoint *>(pDummy);
      pJoint->setPosition(thePos);
      pJoint->setID(getNextId());

      //set the joint's limits
      pJoint->setParameter(0,CIrrOdeJoint::eHiStop, 25.0f);
      pJoint->setParameter(0,CIrrOdeJoint::eLoStop, -9.0f);  //strange thing: lower low stop makes this joint explode
      pJoint->setParameter(0,CIrrOdeJoint::eBounce,  0.9f);

      //add a motor (if wanted)
      if (bMotor) {
        CIrrOdeMotor *pMotor=reinterpret_cast<CIrrOdeMotor *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_MOTOR_NAME),pJoint));
        pMotor->setVelocity(-3.0f);
        pMotor->setForce(500.0f);
        pMotor->setEnabled(true);
        pMotor->drop();
      }
      break;

    //add a new hinge-2 joint
    case IRR_ODE_JOINT_HINGE2_ID:
      pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_HINGE2_NAME),pBody1);
      pJoint=reinterpret_cast<CIrrOdeJoint *>(pDummy);
      pJoint->setID(getNextId());
      pJoint->setPosition(thePos);

      //calculate the joint's axes
      axis=rot.rotationToDirection(dir);
      ((CIrrOdeJointHinge2 *)pJoint)->setHingeAxis1(axis);
      (rot+vector3df(0,90,0)).rotationToDirection(dir);
      ((CIrrOdeJointHinge2 *)pJoint)->setHingeAxis2(axis);

      pJoint->setParameter(0,CIrrOdeJoint::eHiStop, M_PI/2);
      pJoint->setParameter(0,CIrrOdeJoint::eLoStop,-M_PI/2);
      pJoint->setParameter(0,CIrrOdeJoint::eBounce,1.0f);
      break;

    default:
      printf("unsupported node type!\n");
      return;
  }

  //if the joint is not to be linked to the world we create a second body
  if (!bLinkWorld) {
    CIrrOdeBody *pBody=addNewBody(smgr,driver,pJoint,vector3df(0,-25,0),rot,vector3df(10,10,10),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_SPHERE_ID,false);
    pBody->drop();
  }

  //init physics and drop scene nodes
  pBody1->initPhysics();
  pBody1->drop();
  pJoint->drop();
}

int main(int argc, char** argv) {
  bool bShadows=false,bTerrainTrimesh=false;

  printf("\n\t********************************\n");
  printf(  "\t* Some options to choose from! *\n");
  printf(  "\t********************************\n\n");
  printf("Use shadows (y/n)? ");
  char c=' ';
  do { c=getch(); printf("%c",c); } while (c!='y' && c!='n');
  bShadows=c=='y';
  printf("\nUse trimesh for terrain (y/n)? ");
  do { c=getch(); printf("%c",c); } while (c!='y' && c!='n');
  bTerrainTrimesh=c=='y';
  printf("\nshadows: %s\nterrain trimesh: %s\n\n",bShadows?"YES":"NO",bTerrainTrimesh?"YES":"NO");

  g_pParams[0].setMode(dContactBounce | dContactSoftERP | dContactSoftCFM | dContactSlip1 | dContactSlip2);
  g_pParams[0].setMu(dInfinity); g_pParams[0].setMu2(0); g_pParams[0].setBounce((dReal)0.6);
  g_pParams[0].setBounceVel(1e-9f); g_pParams[0].setSoftCfm(0.005f); g_pParams[0].setSoftErp(0.00005f);
  g_pParams[0].setSlip1(1.0f); g_pParams[0].setSlip2(1.0f);

  g_pParams[1].setMode(dContactBounce | dContactSoftERP | dContactSoftCFM | dContactSlip1 | dContactSlip2);
  g_pParams[1].setMu(dInfinity); g_pParams[1].setMu2(0); g_pParams[1].setBounce((dReal)0.6);
  g_pParams[1].setBounceVel(1.5f); g_pParams[1].setSoftCfm(1e-6f); g_pParams[1].setSoftErp(0.00005f);
  g_pParams[1].setSlip1(1.0f); g_pParams[1].setSlip2(1.0f);

  ISceneNode *pNode;
  IrrlichtDevice *device=createDevice(EDT_DIRECT3D9, dimension2d<u32>(640,480),16,false,bShadows,false,0);
  device->setWindowCaption(L"Hello World! - Irrlicht-ODE Engine Demo");

  IVideoDriver *driver = device->getVideoDriver();
  ISceneManager *smgr = device->getSceneManager();
  IGUIEnvironment *guienv = device->getGUIEnvironment();
  ICursorControl *pCursor=device->getCursorControl();
  pCursor->setVisible(false);

  CIrrOdeSceneNodeFactory cFactory(smgr);
  smgr->registerSceneNodeFactory(&cFactory);

  dimension2d<u32> cSize=driver->getScreenSize();

  //we want a crosshair to aim
  s32 px=(cSize.Width/2)-8,py=(cSize.Height/2)-8;
  guienv->addImage(driver->getTexture("../../data/crosshair.png"),position2d<s32>(px,py));

  //init the ODE
  CIrrOdeManager::getSharedInstance()->initODE();

  //set IrrODE's timer
  ITimer *pTimer=device->getTimer();
  CIrrOdeManager::getSharedInstance()->setTimer(pTimer);

  //create an event receiver and init it
  myEventReceiver theEventReceiver;
  device->setEventReceiver(&theEventReceiver);

  //First we need to create a world for our bodies to be in
  pNode=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_WORLD_NAME),smgr->getRootSceneNode());
  CIrrOdeWorld *worldNode=reinterpret_cast<CIrrOdeWorld *>(pNode);

  //let's set some parameters of the world
  worldNode->setStepSize(0.01f);
  worldNode->setAngularDamping(0.0002f);
  worldNode->setAngularDampingThreshold(0.01f);
  worldNode->setLinearDamping(0.0002f);
  worldNode->setLinearDampingThreshold(0.01f);
  worldNode->setAutoDisableFlag(1);
  worldNode->setAutoDisableAngularThreshold(0.5f);
  worldNode->setAutoDisableLinearThreshold(0.5f);
  worldNode->setAutoDisableTime(1.0f);
  worldNode->setGravity(vector3df(0,-30,0));

  //we want the world to have a terrain. First load an Irrlicht Terrain SceneNode ...
  ITerrainSceneNode *terrain=smgr->addTerrainSceneNode(bTerrainTrimesh?_TERRAIN_LO:_TERRAIN_HI,
                                                       worldNode,
                                                       -1,
                                                       vector3df(0.0f,0.0f,0.0f),
                                                       vector3df(0.0f,0.0f,0.0f),
                                                       vector3df(40.0f,4.4f,40.0f),
                                                       SColor(255,255,255,255),
                                                       5,ETPS_17,4);
  if (terrain) {
    terrain->setMaterialFlag(EMF_LIGHTING, true);
    terrain->setMaterialTexture(0,driver->getTexture("../../data/terrain-texture.jpg"));
  }

  smgr->addLightSceneNode(NULL,vector3df(0,5000,0),SColor(0x80,0xFF,0xF0,0xF0),100000);

  //... next create an IrrODE Heightfield or trimesh as child of the terrain

  if (!bTerrainTrimesh) {
    printf("creating terrain as ODE heightfield ... ");
    pNode=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_HEIGHTFIELD_NAME),terrain);
    CIrrOdeGeomHeightfield *hf=reinterpret_cast<CIrrOdeGeomHeightfield *>(pNode);
    hf->setID(getNextId());
    hf->setSurfaceParameters(&g_pParams[++g_iParamNum%2],0);
    hf->drop();
  }
  else {
    printf("creating terrain as ODE trimesh ... ");
    pNode=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_TRIMESH_NAME),terrain);
    CIrrOdeGeomTrimesh *hf=reinterpret_cast<CIrrOdeGeomTrimesh *>(pNode);
    hf->setID(getNextId());
    hf->setSurfaceParameters(&g_pParams[++g_iParamNum%2],0);
    hf->drop();
  }
  printf("Ready.\n");


  //we place a FPS camera in the center of the terrain
  vector3df camPos=terrain?terrain->getTerrainCenter():vector3df(0,0,0);
  camPos+=vector3df(0,terrain?terrain->getHeight(camPos.X,camPos.Y)+100:200,0);
  ICameraSceneNode *cam=smgr->addCameraSceneNodeFPS();
  smgr->addLightSceneNode(cam,vector3df(0,0,0),SColor(0x80,0xFF,0xF0,0xF0),500.0f);

  //now some static text with information for the user
  //1st: the number of bodies and the number of active bodies
  IGUIStaticText *pActiveBodies=guienv->addStaticText(L"Hello World!",rect<s32>(5,5,105,17),true);
  pActiveBodies->setDrawBackground(true);
  pActiveBodies->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));

  //2nd: information about the body the user aims at
  s32 px1=cSize.Width-155,py1=5,px2=cSize.Width-5,py2=100;
  IGUIStaticText *pBodyInfo=guienv->addStaticText(L"Hello World!",rect<s32>(px1,py1,px2,py2),true);

  pBodyInfo->setDrawBackground(true);
  pBodyInfo->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));

  py1=105; py2=cSize.Height-5;

  //3rd: information about joints attached to a body
  IGUIStaticText *pJoints=guienv->addStaticText(L"Hello World!",rect<s32>(px1,py1,px2,py2),true);
  pJoints->setDrawBackground(true);
  pJoints->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));

  f32 fMinX=terrain->getBoundingBox().MinEdge.X,fMinZ=terrain->getBoundingBox().MinEdge.Z,
      fMaxX=terrain->getBoundingBox().MaxEdge.X,fMaxZ=terrain->getBoundingBox().MaxEdge.Z;

  //let's create an IrrODE event listener for collision events and add it to the IrrODEManager
  myBodyEventListener *pColEvtListener=new myBodyEventListener(driver,pActiveBodies,fMinX,fMinZ,fMaxX,fMaxZ);
  CIrrOdeManager::getSharedInstance()->addEventListener(pColEvtListener);

  //let's create an IrrODE event listener for ray events and add it to the IrrODEManager
  myRayListener *rayListener=new myRayListener(&theEventReceiver,cam,pBodyInfo,pJoints);
  CIrrOdeManager::getSharedInstance()->addEventListener(rayListener);

  //add a ray to the world. Will be updated each frame
  pNode=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_RAY_NAME),worldNode);
  CIrrOdeGeomRay *pRay=reinterpret_cast<CIrrOdeGeomRay *>(pNode);
  pRay->set(cam->getPosition(),vector3df(0,0,1),100.0f);
  pRay->setBackfaceCull(true);

  IGUIStaticText *pHelp=guienv->addStaticText(L"Help",rect<s32>(5,5,635,475),true);
  pHelp->setDrawBackground(true);
  pHelp->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));

  char sHelpFile[10000];
  FILE *f=fopen("../../data/irrOdePlaygroundHelp.txt","rt");
  if (f) {
    fread(sHelpFile,1,10000,f);
    fclose(f);
  }
  else strcpy(sHelpFile,"Can't open \"irrOdePlayground.txt\".");

  stringw sw(sHelpFile);
  pHelp->setText(sw.c_str());

  cam->setPosition(camPos);

  //if the IrrODE world was created we will init physics of the world and any body within here
  if (worldNode) {
    worldNode->setMaterialFlag(EMF_LIGHTING, false);
    worldNode->initPhysics();
    worldNode->setGravity(vector3df(0,-10,0));
  }

  int lastFPS=-1;

  srand(time(NULL));
  while (pTimer->isStopped()) pTimer->start();

  driver->setAmbientLight(SColor(0x80,0xFF,0xFF,0xFF));

  //let's run the loop
  while(device->run()) {
    if (!rayListener->rayHit()) pBodyInfo->setBackgroundColor(SColor(0x80,0xFF,0xFF,0xFF));

    if (theEventReceiver.help()) {
      pHelp->setVisible(true);
      pBodyInfo->setVisible(false);
      pActiveBodies->setVisible(false);
      pJoints->setVisible(false);
      while (!pTimer->isStopped()) pTimer->stop();
    }
    else {
      pBodyInfo->setVisible(true);
      pActiveBodies->setVisible(true);
      pJoints->setVisible(true);
      pHelp->setVisible(false);
      while (pTimer->isStopped()) pTimer->start();
    }

    //add a new sphere
    if (theEventReceiver.newSphere()) {
      f32 fScale=10+(rand()%5); vector3df scale=vector3df(fScale,fScale,fScale);
      CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),scale,(ESCENE_NODE_TYPE)IRR_ODE_GEOM_SPHERE_ID);
      pBody->drop();
    }

    //add a new body
    if (theEventReceiver.newBox()) {
      vector3df scale=vector3df(20+(rand()%10),20+(rand()%10),20+(rand()%10));
      CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),scale,(ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID);
      pBody->drop();
    }

    //add a new cylinder
    if (theEventReceiver.newCylinder()) {
      f32 fRad=20+(rand()%10);
      vector3df scale=vector3df(fRad,fRad,20+(rand()%10));
      CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),scale,(ESCENE_NODE_TYPE)IRR_ODE_GEOM_CYLINDER_ID);
      pBody->drop();
    }

    //add a new capsule
    if (theEventReceiver.newCapsule()) {
      CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),vector3df(20,20,20),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_CAPSULE_ID);
      pBody->drop();
    }

    //add a new trimesh
    if (theEventReceiver.newTrimesh()) {
      CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),vector3df(25,25,25),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_TRIMESH_ID);
      pBody->drop();
    }

    if (theEventReceiver.xKeyDown()) {
      CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),vector3df(10,10,10),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_SPHERE_ID,false);

      IAnimatedMesh *pMesh=smgr->getMesh("../../data/box.3ds");
      IAnimatedMeshSceneNode *pNode=smgr->addAnimatedMeshSceneNode(pMesh,pBody);
      pNode->setScale(vector3df(25.0f,3.0f,3.0f));
      pNode->setRotation(vector3df(45,0,45));
      pNode->setMaterialTexture(0,driver->getTexture("../../data/box0.jpg"));
      pNode->setMaterialFlag(EMF_LIGHTING,false);
      reinterpret_cast<CIrrOdeGeomBox *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),pNode));
      pBody->initPhysics();
      pBody->drop();
    }

    //add a new stack of boxes
    if (theEventReceiver.newStack()) {
      f32 fSize=15,xOff=0;
      vector3df rot=cam->getRotation(),
                diry=rot.rotationToDirection(vector3df(0,1,0)),
                dirz=rot.rotationToDirection(vector3df(0,0,1));

      for (s32 y=4; y>=0; y--) {
        f32 posy=-35*(y-3);
        for (s32 x=0; x<=y; x++) {
          f32 posx=40*(x-3)+xOff;
          vector3df pos=cam->getPosition()+rot.rotationToDirection(vector3df(posx,posy,0));//vector3df(posx,posy,0);
          CIrrOdeBody *pBody=addNewBody(smgr,driver,worldNode,pos,cam->getRotation(),vector3df(fSize,fSize,fSize),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID);
          pBody->drop();
        }
        xOff+=25;
      }
    }

    //add a new "swinging bridge"
    if (theEventReceiver.newBridge()) {
      vector3df dir=cam->getTarget(),
                rot=cam->getAbsoluteTransformation().getRotationDegrees(),pos;

      dir=vector3df(1,0,0);
      pos=vector3df(0,0,24);

      CIrrOdeBody *pBody1=addNewBody(smgr,driver,worldNode,cam->getPosition(),rot,vector3df(30,5,20),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID,false);
      CIrrOdeBody *pRootBody=pBody1;

      vector3df axis=rot.rotationToDirection(dir);
      ISceneNode *pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_HINGE_NAME),pBody1);
      CIrrOdeJointHinge *pJoint1=reinterpret_cast<CIrrOdeJointHinge *>(pDummy);
      pJoint1->setID(getNextId());
      pJoint1->setPosition(-pos);
      pJoint1->setHingeAxis(axis);

      for (s32 x=0; x<9; x++) {
        //add the next joint to the bridge
        pDummy=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_JOINT_HINGE_NAME),pBody1);
        CIrrOdeJointHinge *pJoint2=reinterpret_cast<CIrrOdeJointHinge *>(pDummy);

        //set the joints's ID, position and axis
        pJoint2->setID(getNextId());
        pJoint2->setPosition(pos);
        pJoint2->setHingeAxis(axis);

        //create the next segment
        CIrrOdeBody *pBody2=addNewBody(smgr,driver,pJoint2,pos,vector3df(0,0,0),vector3df(30,5,20),(ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID,false);

        //attach the segment to the joint
        pJoint2->setBody2(pBody2);

        //drop what's not needed any more
        pBody1->drop();
        pJoint2->drop();
        //we need the new segment in the next step
        pBody1=pBody2;
      }

      pJoint1=new CIrrOdeJointHinge(pBody1,smgr,getNextId(),vector3df(12,0,0));
      pJoint1->setHingeAxis(axis);
      pRootBody->initPhysics();
      pBody1->drop();
    }

    //add some joints
    if (theEventReceiver.newFixedJoint ()) addNewJoint(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),(ESCENE_NODE_TYPE)IRR_ODE_JOINT_FIXED_ID      ,theEventReceiver.shiftDown(),false                      );
    if (theEventReceiver.newBallJoint  ()) addNewJoint(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),(ESCENE_NODE_TYPE)IRR_ODE_JOINT_BALL_SOCKET_ID,theEventReceiver.shiftDown(),false                      );
    if (theEventReceiver.newHingeJoint ()) addNewJoint(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),(ESCENE_NODE_TYPE)IRR_ODE_JOINT_HINGE_ID      ,theEventReceiver.shiftDown(),theEventReceiver.ctrlDown());
    if (theEventReceiver.newSliderJoint()) addNewJoint(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),(ESCENE_NODE_TYPE)IRR_ODE_JOINT_SLIDER_ID     ,theEventReceiver.shiftDown(),theEventReceiver.ctrlDown());
    if (theEventReceiver.newPistonJoint()) addNewJoint(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),(ESCENE_NODE_TYPE)IRR_ODE_JOINT_PISTON_ID     ,theEventReceiver.shiftDown(),theEventReceiver.ctrlDown());
    if (theEventReceiver.newHinge2Joint()) addNewJoint(smgr,driver,worldNode,cam->getPosition(),cam->getRotation(),(ESCENE_NODE_TYPE)IRR_ODE_JOINT_HINGE2_ID     ,false                       ,theEventReceiver.ctrlDown());

    //pause the simulation
    if (theEventReceiver.pause())
      if (pTimer->isStopped())
        while ( pTimer->isStopped()) pTimer->start();
      else
        while (!pTimer->isStopped()) pTimer->stop();

    //increase or decrease the simulation's speed
    if (theEventReceiver.incSpeed() && pTimer->getSpeed()<2.0f) pTimer->setSpeed(pTimer->getSpeed()+0.1f);
    if (theEventReceiver.decSpeed() && pTimer->getSpeed()>0.5f) pTimer->setSpeed(pTimer->getSpeed()-0.1f);

    //set the ray to the camera's position and update the target of the ray
    pRay->set(cam->getPosition(),cam->getTarget()-cam->getPosition(),500.0f);

    //step the simulation
    CIrrOdeManager::getSharedInstance()->step();

    //now for the normal Irrlicht stuff ... begin, draw and end scene and update window caption
    driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();

    if (theEventReceiver.drawTerrainGrid())
      CIrrOdeManager::getSharedInstance()->getOdeDevice()->trimeshDrawDebug(7,driver);

    guienv->drawAll();

    driver->endScene();
    int fps = driver->getFPS();

    if (lastFPS != fps) {
      core::stringw str = L"Irrlicht Engine - IrrODE example [";
      str += driver->getName();
      str += "] FPS:";
      str += fps;
      if (pTimer->isStopped())
        str +=", simulation paused";
      else {
        str += ", timer speed:";
        str += pTimer->getSpeed();
      }

      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
  }

  device->drop();

  return 0;
}


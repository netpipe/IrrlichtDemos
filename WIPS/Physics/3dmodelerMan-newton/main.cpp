#include <irrlicht.h>
#include "newton.h"
#include "tumle.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace tumle;

int main(int argc, char** argv)
{

//create Irrlicht device
    IrrlichtDevice *device =
        createDevice(EDT_OPENGL, dimension2d<s32>(640, 480), 16,
            false, false, false, 0);

            if (!device) return 1;

//set the window caption
    device->setWindowCaption(L"Welcome to the motion demo!");
    device->setResizeAble(true);


//pointers to Irrlicht stuff
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    //create the physics manager from the irrlicht device
    IPhysicsManager *pmgr = createPhysicsManager(device);
    if (!pmgr) return 1;

//===================================================================================

// set ambient light
 smgr->setAmbientLight(SColor(0,60,60,60) );

//add logo image
 guienv->addImage(driver->getTexture("media/irrlichtlogo2.png"),
 position2d<int>(500,10));

 //add credits text
 guienv->addStaticText(L"Thanks to the Irrlicht community\n who helped me learn to program with Irrlicht\n also to:\n\n Nikolas Gebhart, for the Irrlicht engine and\n the media files I borrowed.\n\n jonLT, for the tumle wrapper for newton\n\n and petomavar (on turbo squid)\n for the skybox I used. ",
 rect<s32>(10,10,260,122), true);

//add light
 smgr->addLightSceneNode(0, vector3df(-15,5,-105), SColorf(1.0f, 1.0f, 1.0f));

//add a camera
 ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
 cam->setPosition(vector3df(0,100,-500) );

//add particle system
 IParticleSystemSceneNode* ps = smgr->addParticleSystemSceneNode(false);
 IParticleEmitter* em = ps->createBoxEmitter(
 aabbox3d<f32>(-7,0,-7,7,1,7),
 vector3df(0.0f,0.06f,0.0f),
 150,200,
 SColor(0,255,255,255),
 SColor(0,255,255,255),
 800,2000,10,
 dimension2df(10.f,10.f),
 dimension2df(15.f,15.f));
 ps->setEmitter(em);
 em->drop();
 ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
 ps->setMaterialTexture(0, driver->getTexture("media/fire.bmp") );
 ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator (core::vector3df(0,100,0),250.0f);
 ps->addAnimator(anim);
 anim->drop();
 IParticleAffector* effector = ps->createScaleParticleAffector(dimension2df(20,20) );
 ps->addAffector( effector );
 effector->drop();

 //add level object
 ISceneNode* level = smgr->addMeshSceneNode(smgr->getMesh("media/level.obj") );
 if (level)
 {
  level->setScale(vector3df(40,10,40) );
  level->setMaterialTexture(0, driver->getTexture("media/light_grass.bmp") );
  level->setMaterialFlag(EMF_LIGHTING, true);
  level->getMaterial(0).EmissiveColor.set(100,100,100,100);
 }

 //add level physics node
 IPhysicsNode *levelP = pmgr->addPhysicsNode(level, EBT_BOX, 0, vector3df(1000,4,1000));
 //set the texure to grass later

 //add cube node
 ISceneNode *cube = smgr->addCubeSceneNode(10,0,-1,vector3df(0,-20,0));
 cube->setPosition(vector3df(0,100,0) );
 cube->setScale( vector3df(3,3,3) );
 cube->setMaterialTexture(0, driver->getTexture("media/chrome.bmp") );
 IPhysicsNode *cubeP = pmgr->addPhysicsNode(cube, EBT_BOX, 1, vector3df(45,45,45));

 // add marble node
  ISceneNode *marble = smgr->addSphereSceneNode(5);
  marble->setPosition(vector3df(0,50,-10) );
  marble->getMaterial(0).setTexture(0, driver->getTexture("media/red.png") );

 // add marble node2
  ISceneNode *marble2 = smgr->addSphereSceneNode(5);
  marble2->setPosition(vector3df(10,55,-10) );
  marble2->getMaterial(0).setTexture(0, driver->getTexture("media/red.png") );

  // add marble node3
  ISceneNode *marble3 = smgr->addSphereSceneNode(5);
  marble3->setPosition(vector3df(5,60,-9) );
  marble3->getMaterial(0).setTexture(0, driver->getTexture("media/red.png") );

  // add marble node4
  ISceneNode *marble4 = smgr->addSphereSceneNode(5);
  marble4->setPosition(vector3df(12,70,-5) );
  marble4->getMaterial(0).setTexture(0, driver->getTexture("media/red.png") );


 //add marble physics node
  IPhysicsNode *marbleP = pmgr->addPhysicsNode(marble, EBT_SPHERE, 99999, vector3df(10,10,10));
  if (marbleP)
  {
  marbleP->setGravity(vector3df(0,10,0) );
  ps->setParent(marble);
  }

//add marble physics node 2
   IPhysicsNode *marbleP2 = pmgr->addPhysicsNode(marble2, EBT_SPHERE, 99999, vector3df(10,10,10));
  if (marbleP2)
  {
  marbleP2->setGravity(vector3df(0,10,0) );
  ps->setParent(marble2);
  }

//add marble physics node 3
  IPhysicsNode *marbleP3 = pmgr->addPhysicsNode(marble3, EBT_SPHERE, 99999, vector3df(10,10,10));
  if (marbleP3)
  {
  marbleP3->setGravity(vector3df(0,10,0) );
  ps->setParent(marble);
  }

  //add marble physics node 4
  IPhysicsNode *marbleP4 = pmgr->addPhysicsNode(marble4, EBT_SPHERE, 99999, vector3df(10,10,10));
  if (marbleP4)
  {
  marbleP4->setGravity(vector3df(0,10,0) );
  ps->setParent(marble4);
  }

//add sky
ISceneNode* sky = smgr->addSkyBoxSceneNode(
driver->getTexture("media/skyrender0003.bmp"),
driver->getTexture("media/skyrender0006.bmp"),
driver->getTexture("media/skyrender0005.bmp"),
driver->getTexture("media/skyrender0002.bmp"),
driver->getTexture("media/skyrender0001.bmp"),
driver->getTexture("media/skyrender0004.bmp"));

ISceneNodeAnimator* sky_anim = smgr->createRotationAnimator(vector3df(0,0.01f,0));
sky->addAnimator(sky_anim);
sky_anim->drop();

//add another particle system
 IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false);
 IParticleEmitter* em2 = ps2->createBoxEmitter(
 aabbox3d<f32>(-7,0,-7,7,1,7),
 vector3df(0.0f,0.06f,0.0f),
 150,200,
 SColor(0,255,255,255),
 SColor(0,255,255,255),
 8000,20000,10,
 dimension2df(15.f,15.f),
 dimension2df(25.f,25.f));
 ps2->setEmitter(em2);
 em2->drop();
 ps2->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
 ps2->setMaterialTexture(0, driver->getTexture("media/portal1.bmp") );
 ISceneNodeAnimator* anim2 = smgr->createFlyCircleAnimator (vector3df(0,100,100),250.0f);
 ps2->addAnimator(anim2);
 IParticleAffector* aff = ps2->createScaleParticleAffector(dimension2df(200, 200) );
 ps2->addAffector( aff );
 anim2->drop();

 //add sprite object
 IBillboardSceneNode* bill = smgr->addBillboardSceneNode();
 bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
 bill->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp") );
 bill->setMaterialFlag(EMF_LIGHTING, true);
 bill->getMaterial(0).EmissiveColor.set(225,225,225,225);
 bill->setPosition(vector3df(0,300,0) );
 bill->setSize(dimension2df(100,100) );
 //add another sprite
 ISceneNode* bill2 = bill->clone();
 bill2->getMaterial(0).setTexture(0, driver->getTexture("media/portal1.bmp") );
 ISceneNode* bill3 = bill->clone();
 bill3->getMaterial(0).setTexture(0, driver->getTexture("media/particle.bmp") );

 //add rig object
 IAnimatedMeshSceneNode* rig = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/rig.b3d") );
 rig->setAnimationSpeed( 30 );
 ISceneNode* left = rig->getJointNode("left");
 bill->setParent( left );
 ISceneNode* middle = rig->getJointNode("middle");
 bill2->setParent( middle );
 ISceneNode* right = rig->getJointNode("right");
 bill3->setParent( right );

//main loop
//=====================================================================
while(device->run())
{

if ( cam->getPosition().Y < 30 )
{
 cam->setPosition(vector3df(cam->getPosition().X,30,cam->getPosition().Z ) );
}

//=====================================================================

driver->beginScene(true, true, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

driver->endScene();
//update the newton world
pmgr->update();

//=====================================================================

}

    device->drop();

    return 0;
}


#include "GMiniGolfApp.h"

#include "CFollowAnimator.h"

#include "CBulletAnimatorManager.h"
#include "CBulletWorldAnimator.h"
#include "CWorldEditorGUI.h"


//------------------------------------------------------------------------------
//! Ctor
//! Base initialization
GMiniGolfApp::GMiniGolfApp() :
  CGameApp()
{
  // set custom driver type and screen size
  SetDriverType(video::EDT_DIRECT3D9);
  //SetDriverType(video::EDT_OPENGL);
  SetVideoDim(1280, 720);
  SetVideoDim(800, 600);

  iSphereSceneNode = NULL;
  iCameraSceneNode = NULL;

  iBulletPhysicsFactory = NULL;
  iWorldEditorGUI = NULL;
}

//------------------------------------------------------------------------------
//! Dtor
//!
GMiniGolfApp::~GMiniGolfApp()
{
  if (iBulletPhysicsFactory != NULL) { iBulletPhysicsFactory->drop(); iBulletPhysicsFactory = NULL; }
  if (iWorldEditorGUI != NULL) { delete iWorldEditorGUI; iWorldEditorGUI = NULL; }
}

//------------------------------------------------------------------------------
//! Do additional init work after irrlicht device was created
//!
bool GMiniGolfApp::OnCreate()
{
  if (!CGameApp::OnCreate())
  {
    // base class init failed
    return false;
  }

  //! add our private media directory to the file system
  GetDevice()->getFileSystem()->addFolderFileArchive("../media/");

  // load the engine logo
  GetGUIEnvironment()->addImage(
    GetVideoDriver()->getTexture("irrlichtlogo2.png"),
    core::position2d<s32>(10, 10)
    );

  // show the driver logo
  core::position2di pos (GetVideoDim().Width - 128, GetVideoDim().Height - 64);

  switch (GetDriverType())
  {
  case video::EDT_BURNINGSVIDEO:
    GetGUIEnvironment()->addImage(GetVideoDriver()->getTexture("burninglogo.png"), pos);
    break;
  case video::EDT_OPENGL:
    GetGUIEnvironment()->addImage(GetVideoDriver()->getTexture("opengllogo.png"), pos);
    break;
  case video::EDT_DIRECT3D8:
  case video::EDT_DIRECT3D9:
    GetGUIEnvironment()->addImage(GetVideoDriver()->getTexture("directxlogo.png"), pos);
    break;
  }

  // add texts
  GetGUIEnvironment()->addStaticText(
    L"Use arrow keys and mouse to move FPS camera\n"
    L"F2 - save scene, F3 - load scene\n"
    L"WSAD, SPACE - move ball",
    core::rect<s32>(0, 0, 250, 40), true, true, 0, -1, true);


  // Create GUI of World Editor
  iWorldEditorGUI = new CWorldEditorGUI();
  if (!iWorldEditorGUI->Create(GetDevice()))
    return false;

  AddEventReciever(iWorldEditorGUI);


  // Initialize bullet physics
  iBulletPhysicsFactory = new scene::CBulletAnimatorManager(GetSceneManager());



  // initialize dummy world1's node
  //iWorldNode1 = GetSceneManager()->addEmptySceneNode();
  //iWorldNode1->setName("WorldNode");
  //iWorldNode1->setScale(core::vector3df(0.1, 0.1, 0.1));
  //iWorldNode1->updateAbsolutePosition();
  //AddSampleScene(iWorldNode1);

  // 2nd
  iWorldNode2 = GetSceneManager()->addEmptySceneNode();
  iWorldNode2->setName("WorldNode");
  //iWorldNode2->setScale(core::vector3df(0.1, 0.1, 0.1));
  //iWorldNode2->updateAbsolutePosition();
  AddSampleScene(iWorldNode2);


  iCameraSceneNode =
    //GetSceneManager()->addCameraSceneNodeFPS(0, 100.0f, 300.0f, cCameraFPSSceneNodeID, 0, 0, true);
    //GetSceneManager()->addCameraSceneNodeMaya();
    GetSceneManager()->addCameraSceneNodeFPS();
    //GetSceneManager()->addCameraSceneNode();
  iCameraSceneNode->setRotation(core::vector3df(20.833364, -14.875068, 0.000000));
  iCameraSceneNode->setPosition(core::vector3df(53.800972, 23.301916, -174.089157));
  //iCameraSceneNode->setRotation(core::vector3df(20,90,0));
  //iCameraSceneNode->setPosition(core::vector3df(-300,50,350));
  iCameraSceneNode->setName("camera");

  return true;
}

//------------------------------------------------------------------------------
//! Custom per tick update func
//!
void GMiniGolfApp::OnUpdate(u32 timeMs)
{
  iBulletPhysicsFactory->OnUpdate(timeMs);
}

//------------------------------------------------------------------------------
//! Custom render func
//!
void GMiniGolfApp::OnRender()
{
  CGameApp::OnRender();
}

//------------------------------------------------------------------------------
//! Event receiver
//!
bool GMiniGolfApp::OnEvent(const SEvent& event)
{
  bool aHandled = CGameApp::OnEvent(event);
  if (aHandled)
    return true;


  /*
  If the key 'W' or 'S' was left up, we get the position of the scene node,
  and modify the Y coordinate a little bit. So if you press 'W', the node
  moves up, and if you press 'S' it moves down.
  */

  f32 force = 500.0f;
  if (iSphereSceneNode != 0 &&
      event.EventType == irr::EET_KEY_INPUT_EVENT &&
      event.KeyInput.PressedDown)
  {
    switch(event.KeyInput.Key)
    {
      case KEY_KEY_W:
      case KEY_KEY_S:
      {
        core::vector3df currentPos = iSphereSceneNode->getAbsolutePosition();
        core::vector3df followPos = iCameraSceneNode->getAbsolutePosition();
        core::vector3df d = iCameraSceneNode->getTarget();//followPos - currentPos;
        //d = iCameraSceneNode->getTarget() - followPos;
        d.Y = 0;
        d.setLength(event.KeyInput.Key == KEY_KEY_S ? force : -force);
        iSphereBulletAnimator->applyForce(d);
      } return true;
      case KEY_KEY_A:
      case KEY_KEY_D:
      {
        core::vector3df currentPos = iSphereSceneNode->getAbsolutePosition();
        core::vector3df followPos = iCameraSceneNode->getAbsolutePosition();
        core::vector3df d = iCameraSceneNode->getTarget();//(followPos - currentPos);
        //d = iCameraSceneNode->getTarget() - followPos;
        d.setLength(event.KeyInput.Key == KEY_KEY_D ? force : -force);
        f32 x = d.X;
        d.set(-d.Z, 0, x);
        iSphereBulletAnimator->applyForce(d);
      } return true;
      case KEY_SPACE:
      {
        core::vector3df v;
        v.Y = force*3;
        iSphereBulletAnimator->applyForce(v);
      } return true;
      case KEY_F2:
      {
        SaveWorld();
      } return true;
      case KEY_F3:
      {
        LoadWorld();
      } return true;
      case KEY_F11:
      {
        if (iWorldEditorGUI != NULL)
          iWorldEditorGUI->SetActive(!iWorldEditorGUI->IsActive());
      } return true;
      case KEY_ADD:
      {
        f32 fov = iCameraSceneNode->getFOV();
        iCameraSceneNode->setFOV(fov + BPU_PI/2.5/100.0);
      } return true;
      case KEY_SUBTRACT:
      {
        f32 fov = iCameraSceneNode->getFOV();
        iCameraSceneNode->setFOV(fov - BPU_PI/2.5/100.0);
      } return true;
    }
  }

  return false;
}

//------------------------------------------------------------------------------
//! SaveWorld
//!
bool GMiniGolfApp::SaveWorld()
{
  GetSceneManager()->saveScene("scene.xml");

  return true;
}

//------------------------------------------------------------------------------
//! LoadWorld
//!
bool GMiniGolfApp::LoadWorld()
{
  iSphereBulletAnimator->drop();
  GetSceneManager()->getRootSceneNode()->removeAll();
  GetSceneManager()->getRootSceneNode()->removeAnimators();

  GetSceneManager()->loadScene("scene.xml");

  iSphereSceneNode = GetSceneManager()->getSceneNodeFromName("sphere");
  iCameraSceneNode = (scene::ICameraSceneNode*)GetSceneManager()->getSceneNodeFromName("camera");
  GetSceneManager()->setActiveCamera(iCameraSceneNode);

  iSphereBulletAnimator = NULL;
  core::list<scene::ISceneNodeAnimator*>::Iterator it;
  for (it = iSphereSceneNode->getAnimators().begin(); it != iSphereSceneNode->getAnimators().end(); it++)
  {
    if ((*it)->getType() == scene::ESNAT_BULLET_OBJECT)
    {
      iSphereBulletAnimator = (scene::CBulletObjectAnimator*)(*it);
      iSphereBulletAnimator->grab();
      break;
    }
  }


  return true;
}


//------------------------------------------------------------------------------
//! LoadWorld
//!
bool GMiniGolfApp::AddSampleScene(scene::ISceneNode* node)
{
  scene::CBulletWorldAnimatorParams worldParams;
  scene::CBulletObjectAnimatorGeometry geom;
  scene::CBulletObjectAnimatorParams physicsParams;

  //worldParams.gravity.Y = -20;
  scene::CBulletWorldAnimator* bulletWorldAnimator =
    iBulletPhysicsFactory->createBulletWorldAnimator(
    GetSceneManager(),
    node,
    &worldParams
    );
  node->addAnimator(bulletWorldAnimator);
  bulletWorldAnimator->drop();


  // add level mesh
  GetDevice()->getFileSystem()->addZipFileArchive("../media/map-20kdm2.pk3");
  scene::IQ3LevelMesh* aLevelMesh = (scene::IQ3LevelMesh*) GetSceneManager()->getMesh("maps/20kdm2.bsp");
  if (aLevelMesh == NULL)
    return false;

  scene::ISceneNode* aLevelSceneNode =
    GetSceneManager()->addOctTreeSceneNode(aLevelMesh->getMesh(0), node);

  aLevelSceneNode->setPosition(core::vector3df(-1350,-130,-1400));
  //aLevelSceneNode = GetSceneManager()->addMeshSceneNode(aLevelMesh->getMesh(0));
  //aLevelSceneNode->setRotation(core::vector3df(0, 0, 0));
  //aLevelSceneNode->setScale(core::vector3df(70, 70, 70));
  //aLevelSceneNode->setPosition(core::vector3df(0,-170,0));

  // add level static mesh
  geom.type = scene::CBPAGT_CONCAVE_MESH;
  //geom.type = scene::CBPAGT_CONCAVE_GIMPACT_MESH;
  geom.mesh.irrMesh = aLevelMesh->getMesh(0);
  geom.mesh.irrMesh = NULL;
  geom.meshFile = "maps/20kdm2.bsp";

  physicsParams.mass = 0.0f;

  scene::CBulletObjectAnimator* levelAnim =
    iBulletPhysicsFactory->createBulletObjectAnimator(
    GetSceneManager(),
    aLevelSceneNode,
    bulletWorldAnimator->getID(),
    &geom,
    &physicsParams
    );
  aLevelSceneNode->addAnimator(levelAnim);
  levelAnim->drop();



  // add light
  GetSceneManager()->addLightSceneNode(node, core::vector3df(0, 200, 0));



  // add sphere
  f32 radius = 7.0f;
  iSphereSceneNode =
    GetSceneManager()->addMeshSceneNode(GetSceneManager()->getMesh("ball.3ds")->getMesh(0), node);
    //GetSceneManager()->addSphereSceneNode(radius, 32, node);
  //GetSceneManager()->addCubeSceneNode(radius*2);
  iSphereSceneNode->setRotation(core::vector3df(45, 0, 0));
  iSphereSceneNode->setPosition(core::vector3df(-10,0,20));
  iSphereSceneNode->setScale(core::vector3df(radius,radius,radius));
  //iSphereSceneNode->setPosition(core::vector3df(20,-130,150));
  iSphereSceneNode->setName("sphere");
  iSphereSceneNode->setMaterialType(video::EMT_SOLID);
  iSphereSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);

  // add sphere bullet animator
  geom.type = scene::CBPAGT_SPHERE;
  geom.sphere.radius = radius;


  //geom.type = scene::CBPAGT_BOX;
  //geom.box.X = radius;
  //geom.box.Y = radius;
  //geom.box.Z = radius;

  physicsParams.mass = 7.0f;
  physicsParams.gravity = core::vector3df(0, 0, 0);
  physicsParams.friction = 0.6f;

  iSphereBulletAnimator =
    iBulletPhysicsFactory->createBulletObjectAnimator(
    GetSceneManager(),
    iSphereSceneNode,
    bulletWorldAnimator->getID(),
    &geom,
    &physicsParams
    );
  iSphereSceneNode->addAnimator(iSphereBulletAnimator);


  // add pins
  scene::IAnimatedMesh* aPinMesh = GetSceneManager()->getMesh("../media/pin.x");

  core::vector3df aGroupPos(0,-53,0);
  //core::vector3df aGroupPos(40,-155,300);
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < i; j++)
    {
      scene::ISceneNode* aPinSceneNode =
        GetSceneManager()->addMeshSceneNode(aPinMesh->getMesh(0), node);
      aPinSceneNode->setScale(core::vector3df(10, 10, 10));
      aPinSceneNode->setRotation(core::vector3df(0, 0, 0));
      aPinSceneNode->setPosition(aGroupPos + core::vector3df(-j*12, -53, 0 + i*12));
      aPinSceneNode->setPosition(aGroupPos + core::vector3df(-j*12, -0, 0 + i*12));

      //aPinSceneNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

      // add pin mesh
      geom.type = scene::CBPAGT_CONCAVE_GIMPACT_MESH;
      //geom.type = scene::CBPAGT_CONVEX_MESH;
      geom.mesh.irrMesh = NULL;//aPinMesh->getMesh(0);
      geom.meshFile = "../media/pin.x";

      physicsParams.mass = 0.5f;
      physicsParams.friction = 0.8;
      physicsParams.angularDamping = 0.0;
      physicsParams.restitution = 0.1;
      physicsParams.linearDamping = 0.0;
      physicsParams.centerOfMassOffset = core::vector3df(0, 0, 0);
      physicsParams.gravity = core::vector3df(0, 0/*i*1*/, 0);

      scene::CBulletObjectAnimator* aPinBulletPhysicsAnimator =
        iBulletPhysicsFactory->createBulletObjectAnimator(
        GetSceneManager(),
        aPinSceneNode,
        bulletWorldAnimator->getID(),
        &geom,
        &physicsParams
        );
      aPinSceneNode->addAnimator(aPinBulletPhysicsAnimator);
      aPinBulletPhysicsAnimator->drop();
    }


  return true;
}


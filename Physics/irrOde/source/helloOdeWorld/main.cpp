#include <irrlicht.h>
#include <IrrOde.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace ode;

int main(int argc, char** argv) {

  printf("choose your static collision object:\n\n(1) use box\n(2) use quake3 level\n(3) exit\n\n..");

  char c;
  do { c=getchar(); } while (c!='1' && c!='2' && c!='3');
  if (c=='3') return 0;

  IrrlichtDevice *device=createDevice(EDT_DIRECT3D9,dimension2d<u32>(640,480),16,false,false,false,0);

  device->setWindowCaption(L"HelloOdeWorld");

  ISceneManager *smgr = device->getSceneManager();
  IVideoDriver* driver = device->getVideoDriver();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  stringc aTitle=stringc("hello world!");
  printf("%s\n",aTitle.c_str());
  CIrrOdeSceneNodeFactory cFactory(smgr);
  smgr->registerSceneNodeFactory(&cFactory);

  //init the ODE
  CIrrOdeManager::getSharedInstance()->initODE();

  //set IrrODE's timer
  ITimer *pTimer=device->getTimer();
  CIrrOdeManager::getSharedInstance()->setTimer(pTimer);

  ISceneNode *pNode=smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_WORLD_NAME),smgr->getRootSceneNode());
  CIrrOdeWorld *worldNode=reinterpret_cast<CIrrOdeWorld *>(pNode);

  worldNode->setGravity(vector3df(0.0f,-10.0f,0.0f));

  device->getFileSystem()->addZipFileArchive("../../data/map-20kdm2.pk3");

  scene::IAnimatedMesh *mesh=smgr->getMesh("20kdm2.bsp");

  //if the user has entered "2" we'll load the quake3 level...
  CIrrOdeBody *pBody=NULL;
  if (c=='2') {
    if (mesh) {
      //for optimization we use an octree scene node...
      scene::IMeshSceneNode *node=smgr->addOctreeSceneNode(mesh,worldNode); //->addAnimatedMeshSceneNode(mesh,worldNode);
      if (node) {
        node->setPosition(core::vector3df(-1300,-144,-1249));
        //... and for detailed collision detection we use a trimesh.
        CIrrOdeGeomTrimesh *pTri=reinterpret_cast<CIrrOdeGeomTrimesh *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_TRIMESH_NAME),node));

        //One important thing: a static trimesh has one surface for every meshbuffer (aka "material") of the parent scene node
        for (u32 j=0; j<pTri->getSurfaceParametersCount(); j++) {
          pTri->getSurfaceParameters(j)->setBounce(1.0f);
          pTri->getSurfaceParameters(j)->setModeBounce(true);
        }

        pTri->drop();
      }
    }

    /*pBody=reinterpret_cast<CIrrOdeBody *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),worldNode));
    mesh=smgr->getMesh("../../data/sphere.3ds");
    if (mesh) {
      IAnimatedMeshSceneNode *pSydney=smgr->addAnimatedMeshSceneNode(mesh,pBody);
      pBody->setPosition(vector3df(0.0f,15.0f,0.0f));
      pSydney->setScale(vector3df(0.5f,0.5f,0.5f));
      smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),pSydney);
    }
    else printf("Oops!\n");*/
  }
  else {
    IAnimatedMesh *Mesh=smgr->getMesh("../../data/box.3ds");
    IAnimatedMeshSceneNode *Node=smgr->addAnimatedMeshSceneNode(Mesh,worldNode);
    Node->setMaterialTexture(0,driver->getTexture("../../data/box0.jpg"));
    Node->setScale(vector3df(15.0f,1.5f,15.0f));
    Node->setMaterialFlag(EMF_LIGHTING,false);

    CIrrOdeGeomBox *bx=reinterpret_cast<CIrrOdeGeomBox *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_BOX_NAME),Node));
    bx->getSurfaceParameters(0)->setBounce(1.0f);
    bx->getSurfaceParameters(0)->setModeBounce(true);
    bx->drop();
  }

  //first add a body as child of the worldNode
  pBody=reinterpret_cast<CIrrOdeBody *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_BODY_NAME),worldNode));
  pBody->setPosition(vector3df(0.0f,15.0f,0.0f));
  //next load a mesh and add an AnimatedMeshSceneNode
  //as child of the body
  IAnimatedMesh *Mesh=smgr->getMesh("../../data/sphere.3ds");
  IAnimatedMeshSceneNode *Node=smgr->addAnimatedMeshSceneNode(Mesh,pBody);
  Node->setMaterialTexture(0,driver->getTexture("../../data/sphere0.jpg"));
  Node->setMaterialFlag(EMF_LIGHTING,false);

  //as the last part we add a sphere geom as child of the
  //AnimatedMeshSceneNode
  CIrrOdeGeomSphere *pSphere=reinterpret_cast<CIrrOdeGeomSphere *>(smgr->addSceneNode(CIrrOdeSceneNode::nodeNameToC8(IRR_ODE_GEOM_SPHERE_NAME),Node));
  pSphere->setMassTotal(0.5f);
  pSphere->getSurfaceParameters(0)->setBounce(1.0f);
  pSphere->getSurfaceParameters(0)->setModeBounce(true);
  pSphere->drop();

  worldNode->initPhysics();

  ICameraSceneNode *cam=smgr->addCameraSceneNode();
  cam->setPosition(vector3df(-20.0f, 15.0f, -20.0f));
  cam->setTarget(vector3df(0.0f,0.0f,0.0f));

  while (pTimer->isStopped()) pTimer->start();

  while(device->run()) {
    CIrrOdeManager::getSharedInstance()->step();
    driver->beginScene(true, true, SColor(0,200,200,200));

    if (c=='2' && pBody!=NULL) cam->setTarget(pBody->getAbsolutePosition());

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
  }

  device->drop();
  return 0;
}


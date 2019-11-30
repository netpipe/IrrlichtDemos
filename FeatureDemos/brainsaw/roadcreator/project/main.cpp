  #include <irrlicht.h>
  #include <CRoadMeshLoader.h>

using namespace irr;

int main(int argc, char** argv) {
  IrrlichtDevice *device=createDevice(video::EDT_OPENGL,core::dimension2d<u32>(1024,768),32,false,false,false,0);

  video::IVideoDriver  *driver=device->getVideoDriver();
  scene::ISceneManager *smgr  =device->getSceneManager();
  gui::IGUIEnvironment *guienv=device->getGUIEnvironment();

  //Create a CRaodMeshLoader instand and register it...
  CRoadMeshLoader *pLoader=new CRoadMeshLoader(device);
  smgr->addExternalMeshLoader(pLoader);

  //...and you can simply load the road files.
  scene::IAnimatedMesh *mesh=smgr->getMesh("data/roads/weird.road");
  scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode( mesh );

  if (node) {
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setFrameLoop(0, 310);
  }

  smgr->addCameraSceneNodeFPS();

  while(device->run()) {
    driver->beginScene(true,true,video::SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
  }

  device->drop();

  return 0;
}


#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")

using namespace irr;
using namespace scene;

int main()
{
  IrrlichtDevice* device =
     createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480), 16, false, true);
  if (device == 0)
    return 1; // could not create selected driver.

  video::IVideoDriver* videoDriver = device->getVideoDriver();
  scene::ISceneManager* sceneManager = device->getSceneManager();

  // make the camera
  scene::ICameraSceneNode* camera =
    sceneManager->addCameraSceneNodeFPS();

  scene::IAnimatedMesh* animated =
    sceneManager->getMesh("../../media/dwarf.x");

  scene::IAnimatedMeshSceneNode* dwarf =
    sceneManager->addAnimatedMeshSceneNode(animated);
  dwarf->setAnimationSpeed(30);

  scene::ISceneNodeAnimator* anim;

  anim = sceneManager->createFlyCircleAnimator(core::vector3df(0, 0, 0), 30.f);
    dwarf->addAnimator(anim);
  anim->drop();

  scene::ILightSceneNode* light =
    sceneManager->addLightSceneNode(0);

  scene::IBillboardSceneNode* bill =
    sceneManager->addBillboardSceneNode(light);
  bill->setMaterialFlag(video::EMF_LIGHTING, false);
  bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
  bill->setMaterialTexture(0, videoDriver->getTexture("../../media/particlewhite.bmp"));

  anim = sceneManager->createFlyCircleAnimator(core::vector3df(0, 80, 30), 7.f, -.001f);
    light->addAnimator(anim);
  anim->drop();

  const core::plane3df plane(0, 0, 0, 0, 1, 0);
  //   sceneManager->setShadowColor(video::SColor(150,0,0,0));
  while (device->run())
  {
    if (device->isWindowActive())
    {
      if (videoDriver->beginScene(true, true, video::SColor(255, 100, 100, 100)))
      {
        sceneManager->drawAll();

        scene::IMesh* mesh =
          animated->getMesh(dwarf->getFrameNr());

        video::SMaterial r;
        r.Lighting = false;
        // r.Textures[0] = videoDriver->getTexture("../../media/1.png"); ;

        videoDriver->setMaterial(r);
        core::matrix4 m;
        m.buildShadowMatrix(light->getAbsolutePosition(), plane, 0.f);
        m *= ((ISceneNode*)dwarf)->getAbsoluteTransformation();
        videoDriver->setTransform(video::ETS_WORLD, m);

        u32 b;
        for (b = 0; b < mesh->getMeshBufferCount(); ++b)
          videoDriver->drawMeshBuffer(mesh->getMeshBuffer(b));

        videoDriver->endScene();
      }

    }
  }

  device->drop();

  return 0;
}

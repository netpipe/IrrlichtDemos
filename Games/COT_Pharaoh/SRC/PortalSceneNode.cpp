
#include "PortalSceneNode.h"

PortalSceneNode::PortalSceneNode() {}

PortalSceneNode::PortalSceneNode(IrrlichtDevice* dev, vector3df pos, vector3df targetPos, vector3df targetTarg) {

      device = dev;
//      soundDevice = sndDev;
      smgr = device->getSceneManager();
      driver = device->getVideoDriver();
      portalActive = false;

      // RTT Portal
	  mesh = smgr->getMesh("Meshes/box.ms3d");
	  portal = smgr->addMeshSceneNode(mesh->getMesh(0));
      portal->setRotation(vector3df(0,0,180));
      portal->setMaterialFlag(EMF_LIGHTING, false);
      portal->setPosition(pos + vector3df(0,0,4));
      portal->setScale(vector3df(17, 17, 17));
      rt = driver->createRenderTargetTexture(dimension2d<s32>(512,512));
      portal->setMaterialTexture(0, rt); // set material of cube to render target
      portal->setTriangleSelector(smgr->createTriangleSelector(mesh->getMesh(0), portal));

	  // Animated Portal
	  /*array<ITexture*> portalTextures;
	  for (int i = 1; i <= 7; i++)	{
	      char tmp[20];
		  sprintf(tmp, "Images/portal%d.png", i);
		  portalTextures.push_back(driver->getTexture(tmp));
      }
	  ISceneNodeAnimator* anim = smgr->createTextureAnimator(portalTextures, 100);
	  portal->setMaterialTexture(0, portalTextures[0]);
	  portal->setMaterialType(EMT_REFLECTION_2_LAYER);
	  portal->addAnimator(anim);
	  anim->drop();*/

	  // Portal outer
      ISceneNode* node = smgr->addCubeSceneNode(204);
      node->setPosition(pos);
	  node->setMaterialFlag(EMF_LIGHTING, false);
	  node->setMaterialTexture(0, driver->getTexture("Images/portalouter.png"));
	  node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

	  // Create texture animated portal inner
	  for (int i = 1; i <= 11; i++)	{
	      char tmp[27];
		  sprintf(tmp, "Images/portalinner%d.png", i);
		  portalInnerTextures.push_back(driver->getTexture(tmp));
      }
	  // Create texture animated portal inner
	  for (int i = portalInnerTextures.size()-1; i >= 0; i--)
	      portalInnerTexturesBackwards.push_back(portalInnerTextures[i]);

      // Portal inner
      portalInner = smgr->addCubeSceneNode(204);
      portalInner->setPosition(pos);
	  portalInner->setMaterialFlag(EMF_LIGHTING, false);
	  portalInner->setMaterialTexture(0, driver->getTexture("Images/portalinner1.png"));
	  portalInner->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

	  rttCamera = smgr->addCameraSceneNode();
      rttCamera->setPosition(targetPos);
      rttCamera->setTarget(targetTarg);
      //rttCamera->setAspectRatio(5/3);
      ///rttCamera->setInputReceiverEnabled(false);

}

ITriangleSelector* PortalSceneNode::getTriangleSelector() {

     return portal->getTriangleSelector();

}

void PortalSceneNode::render(ICameraSceneNode* camera) {

     // 3D sound
 //    if (portalHum);
//        portalHum->setVolume(1/(portal->getPosition().getDistanceFrom(camera->getPosition())/300));

     // Render portal
     if (portal->isVisible()) {
        vector3df vec = line3d<float>(camera->getPosition(), portal->getTransformedBoundingBox().getCenter()).getVector();
        rttCamera->setTarget(rttCamera->getPosition()+vec);
        driver->setRenderTarget(rt, true, true, video::SColor(0,0,0,255));
        smgr->setActiveCamera(rttCamera);
        smgr->drawAll();
        driver->clearZBuffer();
        driver->setRenderTarget(0);
        smgr->setActiveCamera(camera);
     }

}

void PortalSceneNode::togglePortalActive(IMetaTriangleSelector* metaSelector) {

     if (!portalActive) {
        portalActive = true;
        metaSelector->removeTriangleSelector(portal->getTriangleSelector());
        ISceneNodeAnimator* anim = device->getSceneManager()->createTextureAnimator(portalInnerTextures, 80, false);
        portalInner->addAnimator(anim);
      //  anim->drop();
//        if (portalHum)
//           portalHum->play();
//        else {
//             portalHum = OutputStreamPtr(OpenSound(soundDevice,"Audio/portalHum.wav", true));
//             portalHum->setRepeat(true);
//             portalHum->play();
 //       }
     } else {
         portalActive = false;
         metaSelector->addTriangleSelector(portal->getTriangleSelector());
         ISceneNodeAnimator* anim = device->getSceneManager()->createTextureAnimator(portalInnerTexturesBackwards, 80, false);
         portalInner->addAnimator(anim);
         anim->drop();
//         portalHum->stop();
     }
//     OutputStreamPtr sound = OutputStreamPtr(OpenSound(soundDevice,"Audio/openportal.wav", false));
 //    sound->setVolume(0.25f);
 //    sound->play();

}

vector3df PortalSceneNode::getPosition() {

     return portal->getPosition();

}

ISceneNodeAnimatorCollisionResponse* PortalSceneNode::checkCameraInside(ICameraSceneNode* camera, IMetaTriangleSelector* metaSelector, ISceneNodeAnimatorCollisionResponse* camAnim) {

     if (portal->getTransformedBoundingBox().isPointInside(camera->getPosition())) {
//        OutputStreamPtr sound = OutputStreamPtr(OpenSound(soundDevice,"Audio/teleport.wav", false));
//        sound->setVolume(0.5f);
 //       sound->play();
        camAnim->drop();
        camera->removeAnimators();
        camera->setPosition(rttCamera->getPosition());
        camera->setTarget(rttCamera->getTarget());
        ISceneNodeAnimatorCollisionResponse* anim = smgr->createCollisionResponseAnimator(metaSelector, camera, core::vector3df(30,60,30), core::vector3df(0,-50,0));
        camera->addAnimator(anim);
        return anim;
     }

     return camAnim;

}

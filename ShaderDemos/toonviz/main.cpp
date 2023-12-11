#include <irrlicht.h>

//http://irrlicht.sourceforge.net/forum/viewtopic.php?t=39860
using namespace irr;

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

   virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
   {
      f32 silhouette = 0.2;
      services->setPixelShaderConstant("silhouetteThreshold", (&silhouette), 1);

      int texture = 0;
      services->setPixelShaderConstant("tex", (float*) (&texture), 1);
   }

};

int main()
{
   IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024, 768));
   if (device == 0)
      return 1;

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();

   video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
   s32 newMaterialType1 = 0;
   io::path pixelShader = "./media/toon.frag";
   io::path vertexShader = "./media/toon.vert";
   if (gpu)
   {
      MyShaderCallBack* mc = new MyShaderCallBack();

      newMaterialType1 = gpu->addHighLevelShaderMaterialFromFiles(vertexShader, "vertexMain", video::EVST_VS_1_1, pixelShader, "pixelMain", video::EPST_PS_1_1, mc, video::EMT_SOLID);

      mc->drop();
   }

   scene::ISceneNode * node = smgr->addSphereSceneNode();
   if (node)
   {
      node->setPosition(core::vector3df(0,0,30));
      node->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
      node->setMaterialType((video::E_MATERIAL_TYPE) newMaterialType1);
   }

   scene::ISceneNode* node2 = smgr->addCubeSceneNode();
   if (node2)
   {
      node2->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));
      node2->setMaterialType((video::E_MATERIAL_TYPE) newMaterialType1);
      scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(core::vector3df(0,0,30), 20.0f);
      if (anim)
      {
         node2->addAnimator(anim);
         anim->drop();
      }
   }

   scene::IAnimatedMeshSceneNode* node3 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../../media/ninja.b3d"));
   if (node3)
   {
      node3->setFrameLoop(0, 13);
      node3->setAnimationSpeed(10);
      node3->setScale(core::vector3df(2.f,2.f,2.f));
      node3->setRotation(core::vector3df(0,-90,0));
      node3->setMaterialType((video::E_MATERIAL_TYPE) newMaterialType1);
   }

   scene::ISceneNode* node4 = smgr->addLightSceneNode(0, core::vector3df(0,0,0), video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
   if (node4)
   {
      scene::ISceneNodeAnimator* anim2 = 0;
      anim2 = smgr->createFlyCircleAnimator (core::vector3df(0,150,0),250.0f);
      node4->addAnimator(anim2);
      anim2->drop();
   }

   scene::ISceneNode* node5 = smgr->addBillboardSceneNode(node4, core::dimension2d<f32>(50, 50));
   if (node5)
   {

      node5->setMaterialFlag(video::EMF_LIGHTING, false);
      node5->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
      node5->setMaterialTexture(0, driver->getTexture("../../media/particlewhite.bmp"));
   }

   smgr->addCameraSceneNodeFPS();
   device->getCursorControl()->setVisible(false);
node->getMaterial(0).Shininess = 20.0f;
node2->getMaterial(0).Shininess = 20.0f;
node3->getMaterial(0).Shininess = 20.0f;
node4->getMaterial(0).Shininess = 20.0f;
node5->getMaterial(0).Shininess = 20.0f;
   while(device->run())
   {
      driver->beginScene(true, true, video::SColor(255,113,113,133));

      smgr->drawAll();

      driver->endScene();
   }

   device->drop();
   return 0;
}

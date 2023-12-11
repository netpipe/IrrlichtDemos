//Author : anirul
//Licence : BSD


#include <irrlicht.h>
#include "CSceneNodeAnimatorCameraArcball.h"


//Create icosahedron class
class CIcoSceneNode : public irr::scene::ISceneNode {
   irr::core::aabbox3d<irr::f32> Box;
   irr::video::S3DVertex Vertices[12];
   irr::u16 Indices[60];
   irr::video::SMaterial Material;
public :
   CIcoSceneNode(
      irr::scene::ISceneNode* parent,
      irr::scene::ISceneManager* mgr,
      irr::s32 id)
      :   irr::scene::ISceneNode(parent, mgr, id)
   {
      Material.Wireframe = false;
      Material.Lighting = false;
      // generate icosahedron
      float t = (1 + sqrtf(5.0f))/2.0f;
      float s = sqrtf(1 + t*t);
      irr::video::SColor red(0xffff0000);
      irr::video::SColor green(0xff00ff00);
      irr::video::SColor blue(0xff0000ff);
      // create the 12 vertices
      Vertices[0] = irr::video::S3DVertex(irr::core::vector3df(t/s, 1/s, 0), irr::core::vector3df(t/s, 1/s, 0), red, irr::core::vector2df(0.0f, 0.0f));
      Vertices[1] = irr::video::S3DVertex(irr::core::vector3df(-t/s, 1/s, 0), irr::core::vector3df(-t/s, 1/s, 0), green, irr::core::vector2df(0.0f, 0.0f));
      Vertices[2] = irr::video::S3DVertex(irr::core::vector3df(t/s, -1/s, 0), irr::core::vector3df(t/s, -1/s, 0), blue, irr::core::vector2df(0.0f, 0.0f));
      Vertices[3] = irr::video::S3DVertex(irr::core::vector3df(-t/s, -1/s, 0), irr::core::vector3df(-t/s, -1/s, 0), red, irr::core::vector2df(0.0f, 0.0f));
      Vertices[4] = irr::video::S3DVertex(irr::core::vector3df(1/s, 0, t/s), irr::core::vector3df(1/s, 0, t/s), green, irr::core::vector2df(0.0f, 0.0f));
      Vertices[5] = irr::video::S3DVertex(irr::core::vector3df(1/s, 0, -t/s), irr::core::vector3df(1/s, 0, -t/s), blue, irr::core::vector2df(0.0f, 0.0f));
      Vertices[6] = irr::video::S3DVertex(irr::core::vector3df(-1/s, 0, t/s), irr::core::vector3df(-1/s, 0, t/s), red, irr::core::vector2df(0.0f, 0.0f));
      Vertices[7] = irr::video::S3DVertex(irr::core::vector3df(-1/s, 0, -t/s), irr::core::vector3df(-1/s, 0, -t/s), green, irr::core::vector2df(0.0f, 0.0f));
      Vertices[8] = irr::video::S3DVertex(irr::core::vector3df(0, t/s, 1/s), irr::core::vector3df(0, t/s, 1/s), blue, irr::core::vector2df(0.0f, 0.0f));
      Vertices[9] = irr::video::S3DVertex(irr::core::vector3df(0, -t/s, 1/s), irr::core::vector3df(0, -t/s, 1/s), red, irr::core::vector2df(0.0f, 0.0f));
      Vertices[10] = irr::video::S3DVertex(irr::core::vector3df(0, t/s, -1/s), irr::core::vector3df(0, t/s, -1/s), green, irr::core::vector2df(0.0f, 0.0f));
      Vertices[11] = irr::video::S3DVertex(irr::core::vector3df(0, -t/s, -1/s), irr::core::vector3df(0, -t/s, -1/s), blue, irr::core::vector2df(0.0f, 0.0f));
      Box.reset(Vertices[0].Pos);
      for (irr::s32 i = 1; i < 12; ++i)
         Box.addInternalPoint(Vertices[i].Pos);
      // create the 20 triangles
      Indices[0] = 0; Indices[1] = 8; Indices[2] = 4;
      Indices[3] = 1; Indices[4] = 10; Indices[5] = 7;
      Indices[6] = 2; Indices[7] = 9; Indices[8] = 11;
      Indices[9] = 7; Indices[10] =  3; Indices[11] =  1;
      Indices[12] = 0; Indices[13] =  5; Indices[14] =  10;
      Indices[15] = 3; Indices[16] =  9; Indices[17] =  6;
      Indices[18] = 3; Indices[19] =  11; Indices[20] = 9;
      Indices[21] = 8; Indices[22] =  6; Indices[23] =  4;
      Indices[24] = 2; Indices[25] =  4; Indices[26] =  9;
      Indices[27] = 3; Indices[28] =  7; Indices[29] =  11;
      Indices[30] = 4; Indices[31] =  2; Indices[32] =  0;
      Indices[33] = 9; Indices[34] =  4; Indices[35] =  6;
      Indices[36] = 2; Indices[37] =  11; Indices[38] = 5;
      Indices[39] = 0; Indices[40] =  10; Indices[41] = 8;
      Indices[42] = 5; Indices[43] =  0; Indices[44] =  2;
      Indices[45] = 10; Indices[46] = 5; Indices[47] =  7;
      Indices[48] = 1; Indices[49] =  6; Indices[50] =  8;
      Indices[51] = 1; Indices[52] =  8; Indices[53] =  10;
      Indices[54] = 6; Indices[55] =  1; Indices[56] =  3;
      Indices[57] = 11; Indices[58] = 7; Indices[59] =  5;
   }

   virtual void OnRegisterSceneNode() {
      if (IsVisible)
         SceneManager->registerNodeForRendering(this);
      ISceneNode::OnRegisterSceneNode();
   }

   virtual void render() {
      irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

      driver->setMaterial(Material);
      driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
      driver->drawVertexPrimitiveList(
         &Vertices[0], 12,
         &Indices[0], 20,
         irr::video::EVT_STANDARD,
         irr::scene::EPT_TRIANGLES,
         irr::video::EIT_16BIT);
   }

   virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const {
      return Box;
   }

   virtual irr::u32 getMaterialCount() const {
      return 1;
   }

   virtual irr::video::SMaterial& getMaterial(irr::u32 i) {
      return Material;
   }
};

int main(int ac, char** av) {
   irr::IrrlichtDevice* device = irr::createDevice(
      irr::video::EDT_OPENGL,
      irr::core::dimension2d<irr::u32>(1024, 768),32,0);
   if (!device)
      return -1;

   device->setWindowCaption(L"IcoSceneNode + Arcball - Demo");

   irr::video::IVideoDriver* driver = device->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();

   {
      irr::scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(
         0,
         irr::core::vector3df(0, 0, 4),
         irr::core::vector3df(0, 0, 0));
      if (camera) {
         irr::scene::CSceneNodeAnimatorCameraArcball* snaca =
            new irr::scene::CSceneNodeAnimatorCameraArcball(
               device->getCursorControl(),
               camera);
         irr::scene::ISceneNodeAnimator* anim = dynamic_cast<irr::scene::ISceneNodeAnimator*>(snaca);
         camera->addAnimator(anim);
         anim->drop();
      }
   }

   CIcoSceneNode* ico = new CIcoSceneNode(smgr->getRootSceneNode(), smgr, 666);
   ico->drop();

   while (device->run()) {
      driver->beginScene(true, true, irr::video::SColor(0, 100, 100, 100));
      smgr->drawAll();
      driver->endScene();
      device->sleep(15);
   }
   device->drop();
   return 0;
}

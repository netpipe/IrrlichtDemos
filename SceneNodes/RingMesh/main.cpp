//Author : Bitplane

#include <irrlicht.h>
using namespace irr;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IMeshBuffer* createRingMesh(f32 radius, f32 width, u32 sections)
{
    CMeshBuffer<S3DVertex>* mb = new CMeshBuffer<S3DVertex>();
    mb->Vertices.set_used(sections*2 + 2); // 2 new verts per section, plus dupe at the start/end
    mb->Indices.set_used(sections*6); // two triangles per section

    // radians per segment
    f32 segRadians = (core::PI*2) / f32(sections);
    f32 segTCoord = 1.0 / f32(sections);

    // position vertices
    for (u32 i=0; i < sections; ++i)
    {
        const vector2df outer = vector2df(sin(segRadians*f32(i)), cos(segRadians*f32(i))) * radius;
        const vector2df inner = vector2df(sin(segRadians*f32(i)), cos(segRadians*f32(i))) * (radius-width);

        S3DVertex &v1 = mb->Vertices[i*2 +0];
        S3DVertex &v2 = mb->Vertices[i*2 +1];
        v1.Color.set(255, 255, 255, 255);
        v1.Normal.set(0, 1.0, 0);
        v1.Pos.set(outer.X, 0, outer.Y);
        v1.TCoords.set(1.0, segTCoord * f32(i));

        v2.Color.set(255,255,255,255);
        v2.Normal.set(0, 1.0, 0);
        v2.Pos.set(inner.X, 0,inner.Y);
        v2.TCoords.set(0.0, segTCoord * f32(i));
    }

    // dupe vertices at the end
    mb->Vertices[sections*2 +0] = mb->Vertices[0];
    mb->Vertices[sections*2 +0].TCoords.set(1.0, 1.0);
    mb->Vertices[sections*2 +1] = mb->Vertices[1];
    mb->Vertices[sections*2 +1].TCoords.set(0.0, 1.0);

    // stitch indices
    u32 v=0;
    for (u32 i=0; i < sections*6; i+=6, v+=2)
    {
        mb->Indices[i+0] = (v+0);
        mb->Indices[i+1] = (v+2);
        mb->Indices[i+2] = (v+3);
        mb->Indices[i+3] = (v+0);
        mb->Indices[i+4] = (v+3);
        mb->Indices[i+5] = (v+1);
    }

    mb->recalculateBoundingBox();
    return mb;
}


int main()
{
   IrrlichtDevice *device =
      createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
         false, false, false, 0);

   if (!device)
      return 1;

   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();
   IGUIEnvironment* guienv = device->getGUIEnvironment();

    // create ring
    IMeshBuffer* mb = createRingMesh(100.f, 25.f, 15);

    // add it to a mesh and recalculate the bounding box
    SMesh *mesh = new SMesh();
    mesh->addMeshBuffer(mb);
    mesh->recalculateBoundingBox();
    mb->drop();

   IMeshSceneNode* node = smgr->addMeshSceneNode(mesh);

   if (node)
   {
      node->setMaterialFlag(EMF_LIGHTING, false);
      // johnny cash
      node->setMaterialTexture( 0, driver->getTexture("fire.bmp") );
   }

   smgr->addCameraSceneNodeFPS();

   while(device->run())
   {
      driver->beginScene(true, true, SColor(255,100,101,140));
      smgr->drawAll();
      driver->endScene();
   }

   device->drop();

   return 0;
}

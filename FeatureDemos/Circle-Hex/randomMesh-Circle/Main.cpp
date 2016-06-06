#include "Main.h"
#include "Hexagon.h"
#include "Grid.h"
#include "circle.h"

using namespace irr;



irr::scene::SMeshBuffer* createCircleFan(irr::u32 verts, irr::f32 radius, const irr::video::SColor& color)
{
   if (verts < 3)
      verts = 3;
   else if (65534 < verts)
      verts = 65534;

   irr::scene::SMeshBuffer* mb = new irr::scene::SMeshBuffer;
   if (!mb)
      return 0;

   irr::video::S3DVertex vertex;
   vertex.Normal.set(0, 1, 0);
   vertex.Color = color;

   // origin vertex
   vertex.Pos.set(0, 0, 0);
   vertex.TCoords.set(.5f, .5f);
   mb->Vertices.push_back(vertex);
   mb->Indices.push_back(0);

   for (irr::s32 n = verts; 0 <= n; --n)
   {
      const irr::f32 x = (irr::core::PI*2.f)*n/verts;

      const irr::f32 dx = sinf(x);
      const irr::f32 dz = cosf(x);

      // vertices are always in the xz plane
      vertex.Pos.set(dx*radius, 0, dz*radius);

      // scale tcoords from -1,1 to 0,1
      vertex.TCoords.set((dx + 1.f) * .5f, (dz + 1.f) * .5f);

      mb->Vertices.push_back(vertex);
      mb->Indices.push_back(n + 1);
   }

   return mb;
}


int main()
{
   IrrlichtDevice* device = createDevice(video::EDT_OPENGL);
   irr::video::IVideoDriver* driver = device->getVideoDriver();
   irr::scene::ISceneManager* smgr = device->getSceneManager();

   smgr->addCameraSceneNodeFPS();

   irr::scene::IMeshBuffer* circleBuffer = createCircleFan(64, 50.0f, irr::video::SColor(255, 255, 255, 255));
   circleBuffer->recalculateBoundingBox();
   circleBuffer->setHardwareMappingHint(irr::scene::EHM_STATIC);

   irr::video::SMaterial mat;
   mat.Lighting = false;

    //camera->setPosition(irr::core::vector3df(0.0, 50.0,-10));

   while (device->run())
   {
      driver->beginScene();
      smgr->drawAll();

      driver->setMaterial(mat);
      driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
      driver->drawIndexedTriangleFan(
         (irr::video::S3DVertex*)circleBuffer->getVertices(),
         circleBuffer->getVertexCount(),
         &circleBuffer->getIndices()[0],
         circleBuffer->getIndexCount() - 2);


      driver->endScene();
   }

   circleBuffer->drop();
   device->drop();

   return 0;
}

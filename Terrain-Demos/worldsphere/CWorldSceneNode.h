/*
 *  CWorldSceneNode.h
 *  creates tessilated Icosahedron.
 *
 *  Created by torleif west on 8/10/10.
 *  Copyright 2010 doubleclique. All rights reserved.
 *
 */
#include <irrlicht.h>
using namespace irr;

class CWorldSceneNode : public scene::ISceneNode
{
public:

   CWorldSceneNode(scene::ISceneNode* parent,
               scene::ISceneManager* mgr,
               s32 id,
               const video::IImage *bumpmap = 0)
   : scene::ISceneNode(parent, mgr, id)
   {

      f32 t = (1+sqrt(5.))/2.;
      f32 tau = t/sqrt(1.+t*t);
      f32 one = 1/sqrt(1.+t*t);

      scene::CVertexBuffer *coreVertices = new scene::CVertexBuffer(video::EVT_2TCOORDS);

      //Alias Maya Platonic Solid Icosahedron vertex Order
      coreVertices->push_back(video::S3DVertex( tau,   0,    -one,   0,0,0, color, 0, 0));
      coreVertices->push_back(video::S3DVertex( tau,  -0,     one,   0,0,0, color, 0, 0));
      coreVertices->push_back(video::S3DVertex(-tau,  -0,     one,    0,0,0, color, 0, 0));
      coreVertices->push_back(video::S3DVertex(-tau,   0,    -one,   0,0,0, color, 0, 0));
      coreVertices->push_back(video::S3DVertex( 0,    -one,   tau,    0,0,0, color, 1, 0));
      coreVertices->push_back(video::S3DVertex( 0,     one,   tau,    0,0,0, color, 1, 1));
      coreVertices->push_back(video::S3DVertex( 0,     one,  -tau,   0,0,0, color, 1, 1));
      coreVertices->push_back(video::S3DVertex( 0,    -one,  -tau,    0,0,0, color, 1, 1));
      coreVertices->push_back(video::S3DVertex(-one , -tau,  -0,      0,0,0, color, 1, 1));
      coreVertices->push_back(video::S3DVertex( one , -tau,  -0,       0,0,0, color, 1, 1));
      coreVertices->push_back(video::S3DVertex( one ,  tau,   0,       0,0,0, color, 1, 1));
      coreVertices->push_back(video::S3DVertex(-one ,  tau,   0,       0,0,0, color, 1, 1));


      /*
         11      11      11      11      11
         /\      /\      /\      /\      /\
        /  \    /  \    /  \    /  \    /  \
       /    \  /    \  /    \  /    \  /    \
      /5     \/10    \/6     \/3     \/2     \5
      ----------------------------------------
      \      /\      /\      /\      /\      /\
       \    /  \    /  \    /  \    /  \    /  \
        \  /    \  /    \  /    \  /    \  /    \
         \/1     \/0     \/7     \/8     \/4     \1
         ----------------------------------------
         \      /\      /\      /\      /\      /
          \    /  \    /  \    /  \    /  \    /
           \  /    \  /    \  /    \  /    \  /
            \/9     \/9     \/9     \/9     \/9

      This would be clockwise

      11,10,5      5,10,1   3,8,7      9,1,0
      11,6,10      10,0,1   3,2,8      9,0,7
      11,3,6      10,6,0   2,4,8      9,7,8
      11,2,3      6,7,0   2,5,4      9,8,4
      11,5,2      6,3,7   5,1,4      9,4,1

      unfortunately,
      irrlicht faces are meant to be counterclockwise !
      */

      scene::CIndexBuffer *coreIndices = new scene::CIndexBuffer(video::EIT_32BIT);

      // indices points for a Icosahedron
      coreIndices->push_back(11);coreIndices->push_back(5);coreIndices->push_back(10);
      coreIndices->push_back(11);coreIndices->push_back(10);coreIndices->push_back(6);
      coreIndices->push_back(11);coreIndices->push_back(6);coreIndices->push_back(3);
      coreIndices->push_back(11);coreIndices->push_back(3);coreIndices->push_back(2);
      coreIndices->push_back(11);coreIndices->push_back(2);coreIndices->push_back(5);


      coreIndices->push_back(5);coreIndices->push_back(1);coreIndices->push_back(10);
      coreIndices->push_back(10);coreIndices->push_back(1);coreIndices->push_back(0);
      coreIndices->push_back(10);coreIndices->push_back(0);coreIndices->push_back(6);
      coreIndices->push_back(6);coreIndices->push_back(0);coreIndices->push_back(7);
      coreIndices->push_back(6);coreIndices->push_back(7);coreIndices->push_back(3);

      coreIndices->push_back(3);coreIndices->push_back(7);coreIndices->push_back(8);
      coreIndices->push_back(3);coreIndices->push_back(8);coreIndices->push_back(2);
      coreIndices->push_back(2);coreIndices->push_back(8);coreIndices->push_back(4);
      coreIndices->push_back(2);coreIndices->push_back(4);coreIndices->push_back(5);
      coreIndices->push_back(5);coreIndices->push_back(4);coreIndices->push_back(1);


      coreIndices->push_back(9);coreIndices->push_back(0);coreIndices->push_back(1);
      coreIndices->push_back(9);coreIndices->push_back(7);coreIndices->push_back(0);
      coreIndices->push_back(9);coreIndices->push_back(8);coreIndices->push_back(7);
      coreIndices->push_back(9);coreIndices->push_back(4);coreIndices->push_back(8);
      coreIndices->push_back(9);coreIndices->push_back(1);coreIndices->push_back(4);


      //make 1280 base patches
      subdivide(3,coreVertices,coreIndices);

      //make backup with unmodified height
      scene::CVertexBuffer *coreVerticesUnmod = new scene::CVertexBuffer(video::EVT_2TCOORDS);
      for(u32 c=0; c<coreVertices->size(); c++)
      {
         coreVerticesUnmod->push_back(coreVertices->operator[](c));
      }
      scene::CIndexBuffer *coreIndicesUnmod = new scene::CIndexBuffer(video::EIT_32BIT);
      for(u32 c=0; c<coreIndices->size(); c++)
      {
         coreIndicesUnmod->push_back(coreIndices->operator[](c));
      }

      //store patch center in patchPos for LOD
      //before applying height
      u16 faces = coreIndices->size() / 3;
      for (int i=0; i<faces; i++)
      {
         u32 index1 = coreIndices->operator[](i * 3);
         u32 index2 = coreIndices->operator[](i * 3 + 1);
         u32 index3 = coreIndices->operator[](i * 3 + 2);

         patchPos[i] = core::plane3d<f32>(coreVertices->operator[](index1).Pos,
                                  coreVertices->operator[](index2).Pos,
                                  coreVertices->operator[](index3).Pos).Normal;
      }

      //calculate Texture and apply Heightmap
      calculateTextureCords(bumpmap,coreVertices,coreIndices);

      // bounding box
      Box.reset(coreVertices->operator[](0).Pos);
      for (s32 i=0; i<coreVertices->size(); ++i)
         Box.addInternalPoint(coreVertices->operator[](i).Pos);


      //Save level 0
      core::array<scene::CDynamicMeshBuffer*> level0;

      for (int i=0; i<faces; i++)
      {
         scene::CVertexBuffer *level0Vertices = new scene::CVertexBuffer(video::EVT_2TCOORDS);
         scene::CIndexBuffer *level0Indices = new scene::CIndexBuffer(video::EIT_32BIT);
         scene::CDynamicMeshBuffer* level0Buffer = new scene::CDynamicMeshBuffer(level0Vertices->getType(),level0Indices->getType());

         u32 index1 = coreIndices->operator[](i * 3);
         u32 index2 = coreIndices->operator[](i * 3 + 1);
         u32 index3 = coreIndices->operator[](i * 3 + 2);

         level0Vertices->push_back(coreVertices->operator[](index1));
         level0Vertices->push_back(coreVertices->operator[](index2));
         level0Vertices->push_back(coreVertices->operator[](index3));

         level0Indices->push_back(0);
         level0Indices->push_back(1);
         level0Indices->push_back(2);

         level0Buffer->setVertexBuffer(level0Vertices);
         level0Buffer->setIndexBuffer(level0Indices);

         level0.push_back(level0Buffer);
      }

      planetBuffer.push_back(level0);

      //tile and save concurrent levels
      //level 7 = 1.2GB
      for (int s=1; s<5; s++)
      {
         core::array<scene::CDynamicMeshBuffer*> level;

         //restore from unmodified Backup;
         coreVertices->drop();
         scene::CVertexBuffer *coreVertices = new scene::CVertexBuffer(video::EVT_2TCOORDS);
         for(u32 c=0; c<coreVerticesUnmod->size(); c++)
         {
            coreVertices->push_back(coreVerticesUnmod->operator[](c));
         }
         coreIndices->drop();
         scene::CIndexBuffer *coreIndices = new scene::CIndexBuffer(video::EIT_32BIT);
         for(u32 c=0; c<coreIndicesUnmod->size(); c++)
         {
            coreIndices->push_back(coreIndicesUnmod->operator[](c));
         }

         subdivide(s,coreVertices,coreIndices);
         calculateTextureCords(bumpmap,coreVertices,coreIndices);

         u32 indiSize = coreIndices->size();
         u32 faces = indiSize / 3;
         u32 facesPerPatch = (u32)pow(4.,s);

         printf("Faces %d\n",faces);
         printf("should %d\n",1280*facesPerPatch);

         u32 current = 0;

         for (u32 p=0; p<1280; p++)
         {

            scene::CVertexBuffer *levelVertices = new scene::CVertexBuffer(video::EVT_2TCOORDS);
            scene::CIndexBuffer *levelIndices = new scene::CIndexBuffer(video::EIT_32BIT);
            scene::CDynamicMeshBuffer* levelBuffer = new scene::CDynamicMeshBuffer(levelVertices->getType(),levelIndices->getType());

            //printf("--------\n");

            for(u32 fp=0; fp<facesPerPatch; fp++)
            {
               u32 index1 = coreIndices->operator[](current * 3);
               u32 index2 = coreIndices->operator[](current * 3 + 1);
               u32 index3 = coreIndices->operator[](current * 3 + 2);

               //printf("%d %d %d \n",(current * 3),(current * 3 + 1),(current * 3 + 2));

               levelVertices->push_back(coreVertices->operator[](index1));
               levelVertices->push_back(coreVertices->operator[](index2));
               levelVertices->push_back(coreVertices->operator[](index3));

               levelIndices->push_back(fp * 3);
               levelIndices->push_back(fp * 3 + 1);
               levelIndices->push_back(fp * 3 + 2);
               current++;
            }

            levelBuffer->setVertexBuffer(levelVertices);
            levelBuffer->setIndexBuffer(levelIndices);
            level.push_back(levelBuffer);

         }
         planetBuffer.push_back(level);
      }

      for(u32 k=0; k<planetBuffer.size(); k++)
      {
         u32 size = planetBuffer[k].size();
         printf("planetBuffer %d size = %d\n",k,size);
      }

      coreIndices->drop();
      coreVertices->drop();
      coreIndicesUnmod->drop();
      coreVerticesUnmod->drop();

      //debug

      for(u32 a=0; a<1280; a++)
      {
         scene::ISceneNode* axis = SceneManager->addCubeSceneNode(0.07);
         axis->setPosition(patchPos[a]);
       //  axis->setMaterialFlag(video::EMF_WIREFRAME,true);
         //axis->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
         axis->setMaterialFlag(video::EMF_LIGHTING,false);
      }


   }
   void handleLOD()
   {

   }
   /**
    * subdivides this Icosahedron
    */
   void subdivide(u32 subdivisions, scene::CVertexBuffer *currentVertices, scene::CIndexBuffer *currentIndices) {

      for(u32 s=0; s<subdivisions; s++)
      {
         u32 faces = currentIndices->size() / 3;

         for (int i = 0; i < faces; i++) {

            u32 index1 = currentIndices->operator[](i * 3);
            u32 index2 = currentIndices->operator[](i * 3 + 1);
            u32 index3 = currentIndices->operator[](i * 3 + 2);

            video::S3DVertex v1 = currentVertices->operator[](index1);
            video::S3DVertex v2 = currentVertices->operator[](index2);
            video::S3DVertex v3 = currentVertices->operator[](index3);

            core::vector3df midpoint1 = v1.Pos.getInterpolated(v2.Pos, .5);
            core::vector3df midpoint2 = v2.Pos.getInterpolated(v3.Pos, .5);
            core::vector3df midpoint3 = v3.Pos.getInterpolated(v1.Pos, .5);

            // each iteration we add three faces, 3 new Vertices
            currentVertices->push_back(video::S3DVertex(midpoint1,   midpoint1.normalize(), color, core::vector2df (0,0)));
            currentVertices->push_back(video::S3DVertex(midpoint2,   midpoint2.normalize(), color, core::vector2df (0,0)));
            currentVertices->push_back(video::S3DVertex(midpoint3,   midpoint3.normalize(), color, core::vector2df (0,0)));

            // add the new face
            currentIndices->push_back(currentVertices->size()-3);
            currentIndices->push_back(currentVertices->size()-2);
            currentIndices->push_back(currentVertices->size()-1);

            // shrink the orginal poloygon to the new smaller size
            currentIndices->setValue(i * 3 + 1, currentVertices->size()-3);
            currentIndices->setValue(i * 3 + 2, currentVertices->size()-1);

            // add the two remaining faces
            currentIndices->push_back(index3);
            currentIndices->push_back(currentVertices->size()-1);
            currentIndices->push_back(currentVertices->size()-2);

            currentIndices->push_back(index2);
            currentIndices->push_back(currentVertices->size()-2);
            currentIndices->push_back(currentVertices->size()-3);
         }
      }
   }

   /**
    * sets the size of the planet
    * @param r <float> the size of the planet
    */
   void setRadius(const f32 r) {
      setScale(core::vector3df (r,r,r));
      radius = r;
   }


   /**
    * returns the size of the planet
    * @return <float> the size of the planet
    */
   f32 getRadius () const {
      return radius;
   }

   virtual void OnRegisterSceneNode()
   {
      if (IsVisible)
         SceneManager->registerNodeForRendering(this);

      ISceneNode::OnRegisterSceneNode();
   }

   /*
    * In the render() method most of the interesting stuff happens: The
    * Scene node renders itself. We override this method and draw the
    * tetraeder.
    */
   virtual void render()
   {
      video::IVideoDriver* driver = SceneManager->getVideoDriver();
      driver->setTransform (video::ETS_WORLD, core::IdentityMatrix);
      driver->setMaterial(this->getMaterial(0));

      u32 level = 3;

      u32 bSize = planetBuffer[level].size();

      for(u32 i = 0; i<bSize; i++)
      {
         driver->drawMeshBuffer(planetBuffer[level][i]);
      }

      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
   }

    /*
    And finally we create three small additional methods.
    irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
    this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
    amount of materials in this scene node (our tetraeder only has one
    material), and irr::scene::ISceneNode::getMaterial() returns the
    material at an index. Because we have only one material here, we can
    return the only one material, assuming that no one ever calls
    getMaterial() with an index greater than 0.
    */
   virtual const core::aabbox3d<f32>& getBoundingBox() const
   {
      return Box;
   }

   virtual u32 getMaterialCount() const
   {
      return 1;
   }

   virtual video::SMaterial& getMaterial(u32 i)
   {
      return Material;
   }

   /**
    * @return longitude of the vector. in radians.
    */
   f32 getLongitude(const core::vector3df &pos) {
      double phi = asin(pos.Y);
      return (phi / core::PI + 0.5);
   }

   /**
    * @return Latitude of the vector. in radians.
    */
   f32 getLatitude(const core::vector3df &pos) {
      double theta = atan2(pos.Z, pos.X);
      return (theta / core::PI + 1.0) * 0.5;
   }

private:
   core::aabbox3d<f32> Box;
   video::SMaterial Material;
   video::SColor color;
   f32 radius;
   core::vector3df patchPos[1280];
   core::array< scene::CDynamicMeshBuffer *> RenderBuffer;
   core::array< core::array<scene::CDynamicMeshBuffer *> > planetBuffer;

   /**
    * sets the normals, the texture cords and makes a seam in the
    * mesh if the texture cords wrapp around the texture
    */

   void calculateTextureCords(const video::IImage *bumpmap,
                        scene::CVertexBuffer *currentVertices,
                        scene::CIndexBuffer *currentIndices)
   {
      // recalculate normals
      for (u32 i=0; i<currentIndices->size(); i+=3)
      {
         const core::vector3df normal = core::plane3d<f32>(currentVertices->operator[](currentIndices->operator[](i+0)).Pos,
                                               currentVertices->operator[](currentIndices->operator[](i+1)).Pos,
                                               currentVertices->operator[](currentIndices->operator[](i+2)).Pos).Normal;

         currentVertices->operator[](currentIndices->operator[](i+0)).Normal = normal;
         currentVertices->operator[](currentIndices->operator[](i+1)).Normal = normal;
         currentVertices->operator[](currentIndices->operator[](i+2)).Normal = normal;
      }

      // create the seam & attach the texture cords
      u32 isize = currentIndices->size();
      u32 vsize = currentVertices->size();
      for (u32 i=0; i<isize; i+=3)
      {
         video::S3DVertex* v1 = &currentVertices->operator[](currentIndices->operator[](i+0));
         video::S3DVertex* v2 = &currentVertices->operator[](currentIndices->operator[](i+1));
         video::S3DVertex* v3 = &currentVertices->operator[](currentIndices->operator[](i+2));
         core::vector3df v1up((getPosition() - v1->Pos).normalize());
         core::vector3df v2up((getPosition() - v2->Pos).normalize());
         core::vector3df v3up((getPosition() - v3->Pos).normalize());
         v1->TCoords = core::vector2df (getLatitude (v1up), getLongitude (v1up));
         v2->TCoords = core::vector2df (getLatitude (v2up), getLongitude (v2up));
         v3->TCoords = core::vector2df (getLatitude (v3up), getLongitude (v3up));
      }

      for (u32 i=0; i<isize; i+=3)
      {
         // only one texture point will cross the texture wrap. We take
         // the point, create a seam, then reset the texture point
         fixTextureCordSeam(i+0, i+1,currentVertices,currentIndices);
         fixTextureCordSeam(i+0, i+2,currentVertices,currentIndices);
         fixTextureCordSeam(i+1, i+0,currentVertices,currentIndices);
         fixTextureCordSeam(i+2, i+0,currentVertices,currentIndices);
      }

      // place bump map on world
      if (bumpmap)
      {
         for (u32 i = 0 ; i < currentVertices->size(); i++)
         {
            core::vector2df cords (currentVertices->operator[](i).TCoords);
            f32 cx = cords.X;
            f32 cy = cords.Y;

            if (cx < 0.f)
               cx += 1.f;

            if (cy < 0.f)
               cy += 1.f;

            u32 width = bumpmap->getDimension ().Width;
            u32 height = bumpmap->getDimension ().Height;

            video::SColor mcolor(bumpmap->getPixel(u32(cx * width) % width, u32(cy * height) % height));

            f32 colorvalue = mcolor.getLuminance () / 255.0 / 10;
            currentVertices->operator[](i).Pos = currentVertices->operator[](i).Pos.getInterpolated(getPosition(), 1.0 + colorvalue);
         }
      }

      // recalculate normals
      for (u32 i=0; i<currentIndices->size(); i+=3)
      {
         const core::vector3df normal = core::plane3d<f32>(currentVertices->operator[](currentIndices->operator[](i+0)).Pos,
                                               currentVertices->operator[](currentIndices->operator[](i+1)).Pos,
                                               currentVertices->operator[](currentIndices->operator[](i+2)).Pos).Normal;

         currentVertices->operator[](currentIndices->operator[](i+0)).Normal = normal;
         currentVertices->operator[](currentIndices->operator[](i+1)).Normal = normal;
         currentVertices->operator[](currentIndices->operator[](i+2)).Normal = normal;
      }
   }

   /**
    * checks the cords, and if they are streched too much it will
    * split the vertex and make a seam.
    */

   void fixTextureCordSeam (const s32 index1,
                      const s32 index2,
                      scene::CVertexBuffer *currentVertices,
                      scene::CIndexBuffer *currentIndices)

   {
      video::S3DVertex* v1 = &currentVertices->operator[](currentIndices->operator[](index1));
      video::S3DVertex* v2 = &currentVertices->operator[](currentIndices->operator[](index2));

      f32 sensitivity = .7f;

      if((v1->TCoords.X - v2->TCoords.X) > sensitivity)
      {
         core::vector2df newTcords(v1->TCoords);
         newTcords.X -= 1.0f;

         if(v1->TCoords.Y - v2->TCoords.Y > sensitivity)
         {
            newTcords.Y -= 1.0f;
         }
         currentVertices->push_back(video::S3DVertex(v1->Pos,v1->Normal, color, newTcords));
         currentIndices->setValue(index1, currentVertices->size() - 1);
         return;
      }
      if((v1->TCoords.Y - v2->TCoords.Y) > sensitivity)
      {
         core::vector2df newTcords(v1->TCoords);
         newTcords.Y -= 1.0f;
         currentVertices->push_back(video::S3DVertex(v1->Pos,v1->Normal, color, newTcords));
         currentIndices->setValue(index1, currentVertices->size() - 1);
      }

   }

};

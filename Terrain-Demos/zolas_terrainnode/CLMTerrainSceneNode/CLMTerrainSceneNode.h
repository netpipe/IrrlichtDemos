
#ifndef __CLMTerrainSceneNode_h__
#define __CLMTerrainSceneNode_h__

#include <irrlicht.h>
#include "ministub.h"

#pragma comment(lib, "libMini.lib")


namespace irr
{
   namespace scene
   {

      using namespace core;
      using namespace video;

      class CLMTerrainSceneNode : public ISceneNode
      {

      protected:

         //////////////////////////////////////////////////////////////////
         // globals for libMini callbacks
         //////////////////////////////////////////////////////////////////

         // prepares a buffer if the buffer is nonempty draws and then empties the buffer
         // this function needs to be called after stub->draw one more time
         static void _BeginFan();

         // fills the buffer with vertecis
         static void _FanVertex(float i,float y,float j);

         // pointer to the scenenode
         static CLMTerrainSceneNode* _self;

         //////////////////////////////////////////////////////////////////

         ministub *stub;
         IVideoDriver* driver;
         SMeshBufferLightMap buffer;      // a buffer for fan vertecis
         S3DVertex2TCoords vertex;      // a default vertex

         // draw the meshbuffer
         void BeginFan();
         // add point (i,y,j) to the meshbuffer
         void FanVertex(float i,float y,float j);

         // point spacing
         float dim;
         // height scaling factor
         float scale;
         // pixel width of the heightmap
         s32 size;
         // resolution
         float res;
         // field of view
         float fovy;
         // aspect ratio
         float aspect;
         // near value
         float nearp;
         // far value
         float farp;

         // the height field
         float *hfield;
         // material
         SMaterial Material;
         // invere of absolute transform to calculate hf coordinates from world coordinates
         matrix4 invAbsoluteTransform;
         // current camera pos in heightfield coordinate
         vector3df pos;
         // current camera target in heightfield coordinate
         vector3df tgt;
         // current camer up in heightfield coordinate
         vector3df up;

         // Bounding box of the heightfield
         mutable aabbox3df Box;

      public:

         // constructor
         CLMTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);

         // destructor
         virtual ~CLMTerrainSceneNode();

         /*!
         \brief create the terrain
         \param heightmap, image with height values
         \param texture, texture used for texturing the terrain
         \param detailmap, not used
         \param gridPointSpacing, spacing between the gridpoints of the heightfield
         \param heightScale, scaling value for image height values
         \param resolution, 1000.0f ... 1000000.0f
         */
         bool create(IImage* heightmap, ITexture* texture, ITexture* detailmap,
            f32 gridPointSpacing, f32 heightScale, f32 resolution);

         // prerender register node for rendering and call child nodes for registering
         virtual void OnPreRender();

         // render the node
         virtual void render() ;

         // post render, animation etc.
         virtual void OnPostRender(u32 timeMs);

         // get the material with given number
         virtual SMaterial& getMaterial(s32 i)
         {
            return Material;
         }

         // get the boundingbox of the node
         const aabbox3df& getBoundingBox() const
         {
            AbsoluteTransformation.transformBox(Box);
            return Box;
         }

         // get the inverse of the absolute transformation
         matrix4& getInvAbsoluteTransformation(){ return invAbsoluteTransform; }

         // get the heightfield data
         f32* getHeightField() { return hfield; }

         // get the length of the square size of the heightfield
         s32 getHeightFieldSize() { return size; }

         // spacing between the sample points
         f32 getPointSpacing() { return dim; }

         // scaling factor to modify the height values
         f32 getHeightScale() { return scale; }

         // get height at grid point (i,j)
         f32 getHeight(int i,int j);

         // get height at heightfield coordinate (x,z)
         // transformation is inv=AbsoluteTransform.getInverse();
         // inv.transformVect(pos);
         f32 getHeight(float x,float z);

         // get fog height at heightfield coordinate (x,z)
         // transformation is inv=AbsoluteTransform.getInverse();
         // inv.transformVect(pos);
         f32 getFogHeight(float x,float z);

         // get normal at heightfield coordinate (x,z)
         // transformation is inv=AbsoluteTransform.getInverse();
         // inv.transformVect(pos);
         vector3df getNormal(float x,float z);

      };

   }

}


#endif


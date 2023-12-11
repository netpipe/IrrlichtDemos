#include "CLMTerrainSceneNode.h"
#include <stdio.h>
#define GRAD_PI 57.29577951f
namespace irr
{

   namespace scene
   {

      using namespace core;
      using namespace video;

      // self pointer for callback
      CLMTerrainSceneNode* CLMTerrainSceneNode::_self=0;
      // prepares a buffer if the buffer is nonempty draws and then empties the buffer
      // this function needs to be called after stub->draw one more time
      void CLMTerrainSceneNode::_BeginFan()
      {
         if(_self) _self->BeginFan();
      }
      // fills the buffer with vertecis
      void CLMTerrainSceneNode::_FanVertex(float i,float y,float j)
      {
         if(_self) _self->FanVertex(i,y,j);
      }

   //////////////////////////////////////////////////////////////////////////


      // constructor
      CLMTerrainSceneNode::CLMTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id):
      ISceneNode(parent,mgr,id)
      {
         driver=SceneManager->getVideoDriver();
         stub=0;
         hfield=0;
         AbsoluteTransformation.getInverse(invAbsoluteTransform);
      }

      CLMTerrainSceneNode::~CLMTerrainSceneNode()
      {
         if(stub) delete stub;
         if(hfield) delete [] hfield;
      }

      //! loads the terrain
      bool CLMTerrainSceneNode::create(IImage* heightmap, ITexture* texture, ITexture* detailmap,
         f32 gridPointSpacing, f32 heightScale, f32 resolution)
      {
         if(stub) delete stub;
         if(hfield) delete [] hfield;

         res=resolution;
         dim=gridPointSpacing;
         scale=heightScale;
         size = heightmap->getDimension().Width;
         hfield = new float[size*size];

         vector3df ext(size*dim,0,size*dim);
         f32 hmin=10000000;
         f32 hmax=-10000000;
         for (int i=0; i<size; i++)
            for (int j=0; j<size; j++)
            {
               hfield[i+j*size] = heightmap->getPixel(i,size-1-j).getRed()*scale;
               if(hmin>hfield[i+j*size]) hmin=hfield[i+j*size];
               if(hmax<hfield[i+j*size]) hmax=hfield[i+j*size];
            }
         Box.MinEdge.set(-ext.X/2.0f,hmin,-ext.Z/2.0f);
         Box.MaxEdge.set(ext.X/2.0f,hmax,ext.Z/2.0f);

         /*
       stub=new ministub(hfield,
            &size,&dim,scale,1.0f,
            CLMTerrainSceneNode::_BeginFan,
            CLMTerrainSceneNode::_FanVertex,
            0,0,0);

//new code
          ministub(float *image, // height field is a float array
            int *size,float *dim,float scale,float cellaspect, // grid definition
            float centerx,float centery,float centerz, // grid center
            void (*beginfan)(),void (*fanvertex)(float i,float y,float j), // mandatory callbacks
            void (*notify)(int i,int j,int s)=0, // optional callback
            float (*getelevation)(int i,int j,int S,void *data)=0, // optional elevation callback
            void *objref=0, // optional data pointer that is passed to the elevation callback
            unsigned char *fogimage=0, // optional ground fog layer
            int fogsize=0,float lambda=0.0f,float displace=0.0f,float attenuation=1.0f,
            void (*prismedge)(float x,float y,float yf,float z)=0,
            void **d2map2=0,int *size2=0,
            float minres=0.0f,float minoff=0.0f,
            float maxd2=0.0f,float sead2=0.0f,
            float seamin=0.0f,float seamax=0.0f,
            int maxcull=0);
        */
float centerx,centery,centerz;


         stub=new ministub(hfield,
            &size,&dim,1.0f,1.0f,
            0,0,0, // grid center
            CLMTerrainSceneNode::_BeginFan,
            CLMTerrainSceneNode::_FanVertex,
            0,0,0);

         vertex.Normal.set(0,1,0);
         vertex.Color.set(128,128,128,128);

         Material.setTexture(1,texture);
         Material.setTexture(2,detailmap);
// try also other material types
    Material.MaterialType=EMT_LIGHTMAP_M2;


         return true;
      }

      // prepares a buffer if the buffer is nonempty draws and then empties the buffer
      // this function needs to be called after stub->draw one more time
      void CLMTerrainSceneNode::BeginFan()
      {
         if(buffer.Vertices.size()>0 && driver)
         {
            driver->drawIndexedTriangleFan(buffer.Vertices.const_pointer(),buffer.Vertices.size(),buffer.Indices.const_pointer(),buffer.Indices.size()-2);
            buffer.Vertices.set_used(0);
            buffer.Indices.set_used(0);
         }
      }
      // fills the buffer with vertecis
      void CLMTerrainSceneNode::FanVertex(float i,float y,float j)
      {
        int detail=256;
         //vertex.Pos.set(dim*i-size/2*dim, y*scale, size/2*dim-dim*j);

         vertex.Pos.set(dim*i-size/2*dim, y, size/2*dim-dim*j);

         //vertex.TCoords.set(1.0f-(float)i/(size-1),1.0f-(float)j/(size-1));
         vertex.TCoords.set(((float)i/(size-1)),(float)j/(size-1));

       float t2i=((int)i%((size)/detail))*
                     ((float)detail/(size-1));
       float t2j=((int)j%((size)/detail))*
                     ((float)detail/(size-1));

       int offi=(int)i/(size/detail);
       int offj=(int)j/(size/detail);

       vertex.TCoords2.set(offi+t2i,offj+t2j);
         buffer.Indices.push_back(buffer.Indices.size());
         buffer.Vertices.push_back(vertex);
      }

      void CLMTerrainSceneNode::OnPreRender()
      {
         if(IsVisible)
            SceneManager->registerNodeForRendering(this,ESNRP_SOLID);

//         ISceneNode::OnPreRender();

         AbsoluteTransformation.getInverse(invAbsoluteTransform);
      }

      void CLMTerrainSceneNode::render()
      {
         ICameraSceneNode* camera=SceneManager->getActiveCamera();
         driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);


         if(stub && camera)
         {

            aspect = camera->getAspectRatio();
            fovy = f32(GRAD_PI)*camera->getFOV();
            nearp = camera->getNearValue();
            farp = camera->getFarValue();

            pos.set(camera->getPosition());
            tgt.set(camera->getTarget());
            up.set(camera->getUpVector());

            invAbsoluteTransform.transformVect(pos);
            invAbsoluteTransform.transformVect(tgt);
            invAbsoluteTransform.transformVect(up);

            vector3df dx=tgt-pos;
            if(dx.getLengthSQ()==0.0)
            {
               dx.Z=-1.0f;
            }
            // make callbacks to this scenenode :)
            _self=this;

            driver->setMaterial(Material);

            stub->draw(res,
               pos.X,pos.Y,pos.Z,
               dx.X,dx.Y,dx.Z,
               up.X,up.Y,up.Z,
               2.0f*fovy,
               aspect,
               nearp,
               farp);

            BeginFan();

            if(DebugDataVisible)
            {
               SMaterial mat;
               mat.Lighting=false;
               mat.Wireframe=true;
               mat.FogEnable=false;
               mat.EmissiveColor=SColor(255,0,255,0);
               mat.AmbientColor=SColor(255,0,255,0);
               mat.DiffuseColor=SColor(255,0,255,0);
               mat.setTexture(1,0);
               mat.setTexture(2,0);
               driver->setMaterial(mat);
               driver->draw3DBox(Box, SColor(255,0,0,255));

               stub->draw(res,
                  pos.X,pos.Y,pos.Z,
                  dx.X,dx.Y,dx.Z,
                  up.X,up.Y,up.Z,
                  2.0f*fovy,
                  aspect,
                  nearp,
                  farp);

               BeginFan();
            }


         }
      }

      void CLMTerrainSceneNode::OnPostRender(u32 timeMs)
      {
//         ISceneNode::OnPostRender(timeMs);

      }


      f32 CLMTerrainSceneNode::getHeight(int i,int j)
      {
         if(stub)
            return stub->getheight(i,j);
         else
            return 0;
      }

      f32 CLMTerrainSceneNode::getHeight(float x,float z)
      {
         if(stub)
            return stub->getheight(x,z);
         else
            return 0;
      }

      f32 CLMTerrainSceneNode::getFogHeight(float x,float z)
      {
         if(stub)
            return stub->getfogheight(x,z);
         else
            return 0;
      }

      vector3df CLMTerrainSceneNode::getNormal(float x,float z)
      {
         vector3df n(0,1,0);
         if(stub)
         {
            f32 nx,ny,nz;
            stub->getnormal(x,z,&nx,&ny,&nz);
            n.set(nx,ny,nz);
         }
         return n;
      }


   }
}

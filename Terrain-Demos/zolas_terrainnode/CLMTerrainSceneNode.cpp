#include "CLMTerrainSceneNode.h"
#define GRAD_PI 57.29577951f
namespace irr
{

   namespace scene
   {

      using namespace irr;
      using namespace scene;
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
      ISceneNode(parent,mgr,id),mesh(0),driver(0)
      {
         driver=SceneManager->getVideoDriver();
         stub=0;
         hfield=0;
         AbsoluteTransformation.getInverse(invAbsoluteTransform);
         setAutomaticCulling(EAC_OFF);
      }

      CLMTerrainSceneNode::~CLMTerrainSceneNode()
      {
         if(stub) delete stub;
         if(hfield) delete [] hfield;
         if(mesh) mesh->drop();
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

         //getMesh();

         Box.MinEdge.set(-ext.X/2.0f,hmin,-ext.Z/2.0f);
         Box.MaxEdge.set(ext.X/2.0f,hmax,ext.Z/2.0f);

         stub=new ministub(hfield,
            &size,&dim,1.0f,1.0f,
            0,0,0,
            CLMTerrainSceneNode::_BeginFan,
            CLMTerrainSceneNode::_FanVertex,
            0,0,0);

         vertex.Normal.set(0,1,0);
         vertex.Color.set(255,255,255,255);

         Material.setTexture(1,texture);
         Material.setTexture(2,detailmap);

         bDrawToMesh=true;
         mesh = new SMesh();
         _self=this;
         stub->draw(
            res,               // resolution
            0,2.0f*hmax,0,         // pos
            0,-1,0,            // dir
            0,0,1,               // up
            -ext.X,               // OTHO fovy
            1,                  // aspect
            1,                  // near
            ext.X);
         BeginFan();
         bDrawToMesh=false;



         bDrawToMesh=false;

         return true;
      }

      // adds the meshbuffer to the mesh
      void CLMTerrainSceneNode::addMeshBufferToMesh()
      {
         if(buffer.Vertices.size()>0)
         {
            // copy the
            SMeshBuffer* mb=new SMeshBuffer();
            u32 n=buffer.Vertices.size();

            for(u32 i=0; i<n; i++)
            {
               // add vertex
               mb->Vertices.push_back(buffer.Vertices[i]);

               if(i>1)
               {
                  mb->Indices.push_back(0);
                  mb->Indices.push_back(i-1);
                  mb->Indices.push_back(i);
               }
            }
            mb->Indices.push_back(0);
            mb->Indices.push_back(n-1);
            mb->Indices.push_back(1);

            mesh->addMeshBuffer(mb);
            mb->drop();
         }
      }

      // prepares a buffer if the buffer is nonempty draws and then empties the buffer
      // this function needs to be called after stub->draw one more time
      void CLMTerrainSceneNode::BeginFan()
      {
         if(buffer.Vertices.size()>0)
         {
            if(driver )
            {
               if(bDrawToMesh)
                  addMeshBufferToMesh();
               else
                  driver->drawIndexedTriangleFan(
                     buffer.Vertices.const_pointer(),
                     buffer.Vertices.size(),
                     buffer.Indices.const_pointer(),
                     buffer.Indices.size()-2);

               buffer.Vertices.set_used(0);
               buffer.Indices.set_used(0);
            }
         }
      }
      // fills the buffer with vertecis
      void CLMTerrainSceneNode::FanVertex(float i,float y,float j)
      {
         vertex.Pos.set(dim*i-size/2*dim, y, size/2*dim-dim*j);
         vertex.TCoords.set((float)i/(size-1),(float)j/(size-1));
         buffer.Indices.push_back(buffer.Indices.size());
         buffer.Vertices.push_back(vertex);
      }

      IMesh* CLMTerrainSceneNode::getMesh()
      {
         return mesh;
      }

      void CLMTerrainSceneNode::OnPreRender()
      {
         if(IsVisible)
            SceneManager->registerNodeForRendering(this,  ESNRP_SOLID);

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
               mat.EmissiveColor=SColor(255,0,0,0);
               mat.MaterialType=EMT_SOLID;
               driver->setMaterial(mat);
               driver->draw3DBox(Box, SColor(255,0,0,255));

               if(mesh)
                  for(int i=0;i<mesh->getMeshBufferCount();i++)
                     driver->drawMeshBuffer(mesh->getMeshBuffer(i));

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

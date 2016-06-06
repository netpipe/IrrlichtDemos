#ifndef __POST_PROCESS_EFFECT_INVERT__
#define __POST_PROCESS_EFFECT_INVERT__
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class IPostProcessInvert : public scene::ISceneNode
{
    public:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[6];//the vertices for the onscreenquad
   video::SMaterial Material;
   video::ITexture* rt0; //the rendertarget
   int mat;

      IPostProcessInvert(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id)
        {
      Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
      Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
      Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
      Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
        }


    void initiate(int sizeW,int sizeH,scene::ISceneManager* smgr)
   {
static stringc vertToonShader =
"varying vec2 vTexCoord;"
"void main(void)"
"{"

  " vec2 Position;"
  " Position.xy = sign(gl_Vertex.xy);"
  " gl_Position = vec4(Position.xy, 0.0, 1.0);"
   "vTexCoord =Position.xy *.5 + .5;"
"}";
static stringc fragToonShader =
"uniform sampler2D texture[2];"
"varying vec2 vTexCoord;"
""
"void main()"
"{"
"  vec4 col=texture2D(texture[0],vTexCoord);"
"   gl_FragColor =vec4(1.0,1.0,1.0,1.0)-col;"
"}";
video::IVideoDriver* driver = smgr->getVideoDriver();
video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
        (
               vertToonShader.c_str(), "main", video::EVST_VS_1_1,
               fragToonShader.c_str(), "main", video::EPST_PS_1_1,
               NULL, (video::EMT_SOLID)
        );


      rt0 = driver->addRenderTargetTexture(core::dimension2d<u32>(sizeW,sizeH));
      Material.Wireframe = false;
      Material.Lighting = false;
      Material.setTexture(0,rt0);
    }




   virtual void OnPreRender(){}

   virtual void render()
   {
      u16 indices[] = {0,1,2,3,4,5};
      video::IVideoDriver* driver = SceneManager->getVideoDriver();

      driver->setMaterial(Material);
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
   }
   //Renders the Post-Process Effect

  virtual u32 getMaterialCount(){return 1;}
  virtual video::SMaterial& getMaterial(s32 i){return (Material);}
  virtual const core::aabbox3d<f32>& getBoundingBox() const{return Box;}
};


#endif


#ifndef __POST_PROCESS_EFFECT_MOTION_GLASS__
#define __POST_PROCESS_EFFECT_MOTION_GLASS__
#include "irrlicht.h"
#include <cstdlib>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



class PPE_Glass_callback: public video::IShaderConstantSetCallBack
{
public:
    float strength;
  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    video::IVideoDriver* driver = services->getVideoDriver();
    services->setVertexShaderConstant("strength", reinterpret_cast<f32*>(&strength),1);
    int var0=0;
    services->setPixelShaderConstant("Texture1", (float*)(&var0), 1);
    int var1=1;
    services->setPixelShaderConstant("Texture2", (float*)(&var1), 1);
	}
};




class IPostProcessGlass : public scene::ISceneNode
{

public:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[6];//the vertices for the onscreenquad
   video::SMaterial Material;//the material used with shader
   video::ITexture* rt0; //the rendertarget/normalmap
   video::ITexture* rt1; //the smgr->drawAll output
   int mat;
   PPE_Glass_callback* callback;

      IPostProcessGlass(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id)
        {
      Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
      Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
      Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
      Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
        }


    void initiate(unsigned int sizeW,unsigned int sizeH,float strength,scene::ISceneManager* smgr)
   {
static stringc vertShader =
"varying vec2 vTexCoord;"
"void main(void)"
"{"
  " vec2 Position;"
  " Position.xy = sign(gl_Vertex.xy);"
  " gl_Position = vec4(Position.xy, 0.0, 1.0);"
   "vTexCoord =Position.xy *.5 + .5;"
"}";

static stringc fragShader =
"uniform float strength;"
"varying vec2 vTexCoord;"
"uniform sampler2D Texture2; "
"uniform sampler2D Texture1;"
""
"void main(void)"
"{"
"   vec2 offset= texture2D( Texture2, vTexCoord).xy;"
"   float d=length(offset);"
"   if(d!=0.0){offset-=0.5*d;}"
"   offset=offset*strength+vTexCoord;"
"   gl_FragColor = texture2D( Texture1, offset );"
//"gl_FragColor=texture2D( Texture1, vTexCoord )+texture2D( Texture2, vTexCoord );"
"}";



video::IVideoDriver* driver = smgr->getVideoDriver();
video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      callback= new PPE_Glass_callback();
      callback->strength=strength;

      Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
        (
               vertShader.c_str(), "main", video::EVST_VS_1_1,
               fragShader.c_str(), "main", video::EPST_PS_1_1,
               callback, (video::EMT_SOLID)
        );
      rt0 = driver->createRenderTargetTexture(core::dimension2d<s32>(sizeW,sizeH));
      rt1 = driver->createRenderTargetTexture(core::dimension2d<s32>(sizeW,sizeH));

      Material.Wireframe = false;
      Material.Lighting = false;
      Material.setTexture(0, rt0);
      Material.setTexture(1,rt1);
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


  virtual u32 getMaterialCount(){return 1;}
  virtual video::SMaterial& getMaterial(s32 i){return (Material);}
  virtual const core::aabbox3d<f32>& getBoundingBox() const{return Box;}
};


#endif


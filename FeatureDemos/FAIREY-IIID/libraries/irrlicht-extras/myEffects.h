#ifndef __POST_PROCESS_EFFECT_MOTION_BLUR__
#define __POST_PROCESS_EFFECT_MOTION_BLUR__
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



class PPE_MotionBlur_callback: public video::IShaderConstantSetCallBack
{
public:
   float strength;
   virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
   {
      video::IVideoDriver* driver = services->getVideoDriver();
      services->setVertexShaderConstant("strength", reinterpret_cast<f32*>(&strength),1);
      int var0=0;
      services->setPixelShaderConstant("texture1", (float*)(&var0), 1);
      int var1=1;
      services->setPixelShaderConstant("texture2", (float*)(&var1), 1);
   }
};




class IPostProcessMotionBlur : public scene::ISceneNode
{

public:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[6];//the vertices for the onscreenquad
   video::SMaterial Material;//the material used with shader
   video::SMaterial Accum;//a simple diffuse material..
   video::ITexture* next; //the rendertarget
   video::ITexture* prev; //the rendertarget
   video::ITexture* accum; //the rendertarget
   int mat;
   PPE_MotionBlur_callback* callback;

   IPostProcessMotionBlur(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id)
   {
      Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[1] = video::S3DVertex(-1.0f, 1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
      Vertices[2] = video::S3DVertex( 1.0f, 1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
      Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
      Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[5] = video::S3DVertex( 1.0f, 1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
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
         "uniform sampler2D texture1;"
         "uniform sampler2D texture2;"
         "varying vec2 vTexCoord;"
         "uniform float strength;"
         "void main()"
         "{"
         " gl_FragColor = mix( texture2D( texture1, vTexCoord ), texture2D( texture2, vTexCoord ), vec4( strength,strength,strength,strength) );"
         "}";


      static stringc fragShader2 =
         "uniform sampler2D texture1;"
         "varying vec2 vTexCoord;"
         "void main()"
         "{"
         " gl_FragColor =texture2D( texture1, vTexCoord );"
         "}";


      video::IVideoDriver* driver = smgr->getVideoDriver();
      video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      callback= new PPE_MotionBlur_callback();
      callback->strength=strength;

      Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
         (
         vertShader.c_str(), "main", video::EVST_VS_1_1,
         fragShader.c_str(), "main", video::EPST_PS_1_1,
         callback, (video::EMT_SOLID)
         );
      Accum.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
         (
         vertShader.c_str(), "main", video::EVST_VS_1_1,
         fragShader2.c_str(), "main", video::EPST_PS_1_1,
         NULL, (video::EMT_SOLID)
         );

      next = driver->addRenderTargetTexture(core::dimension2d<u32>(sizeW,sizeH));
      prev = driver->addRenderTargetTexture(core::dimension2d<u32>(sizeW,sizeH));
      accum = driver->addRenderTargetTexture(core::dimension2d<u32>(sizeW,sizeH));

      Material.Wireframe = false;
      Material.Lighting = false;

      Material.setTexture(0,next);
      Material.setTexture(1,prev);


      Accum.Wireframe = false;
      Accum.Lighting = false;

      Accum.setTexture(0,accum);

   }




   virtual void OnPreRender(){}

   virtual void render()
   {
      u16 indices[] = {0,1,2,3,4,5};
      video::IVideoDriver* driver = SceneManager->getVideoDriver(); //Fills Next
      driver->setRenderTarget(next, true, true, video::SColor(0,0,0,0));
      SceneManager->drawAll();

      driver->setRenderTarget(accum, true, true, video::SColor(0,0,0,0)); //Combine Next&prev in accum
      driver->setMaterial(Material);
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);

      driver->setRenderTarget(prev, true, true, video::SColor(0,0,0,0)); //Write back accum into prev
      driver->setMaterial(Accum);
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
   }

   virtual void renderFinal()
   {
      video::IVideoDriver* driver = SceneManager->getVideoDriver();
      u16 indices[] = {0,1,2,3,4,5};
      driver->setMaterial(Accum);
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
   }


   virtual u32 getMaterialCount(){return 1;}
   virtual video::SMaterial& getMaterial(s32 i){return (Material);}
   virtual const core::aabbox3d<f32>& getBoundingBox() const{return Box;}
};

#endif

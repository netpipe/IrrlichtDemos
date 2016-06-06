#ifndef __POST_PROCESS_EFFECT_BLOOM__
#define __POST_PROCESS_EFFECT_BLOOM__
#include "irrlicht.h"
#include <cstdlib>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct PPE_Bloom_Setup
{
    float sampleDist;
    float strength;
    float multiplier;
};


class PPE_Bloom_callback: public video::IShaderConstantSetCallBack
{
public:
    float sampleDist;
    float strength;
    float multiplier;

  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    video::IVideoDriver* driver = services->getVideoDriver();
    services->setVertexShaderConstant("sampleDist", reinterpret_cast<f32*>(&sampleDist),1);
    services->setVertexShaderConstant("strength", reinterpret_cast<f32*>(&strength),1);
    services->setVertexShaderConstant("multiplier", reinterpret_cast<f32*>(&multiplier),1);
	}
};




class IPostProcessBloom : public scene::ISceneNode
{

public:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[6];//the vertices for the onscreenquad
   video::SMaterial Material;
   video::ITexture* rt0; //the rendertarget
   int mat;
   PPE_Bloom_callback* callback;

      IPostProcessBloom(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id)
        {
      Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
      Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
      Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
      Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
        }


    void initiate(unsigned int sizeW,unsigned int sizeH,PPE_Bloom_Setup data,scene::ISceneManager* smgr)
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
"uniform sampler2D texture[2];"
"varying vec2 vTexCoord;"
"uniform float sampleDist;"
"uniform float strength;"
"uniform float multiplier;"
"void main()"
"{"
"   vec2 samples[12]; "
"   samples[0]  = vTexCoord + sampleDist * vec2(-0.326212, -0.405805);"
"   samples[1]  = vTexCoord + sampleDist * vec2(-0.840144, -0.073580);"
"   samples[2]  = vTexCoord + sampleDist * vec2(-0.695914,  0.457137);"
"   samples[3]  = vTexCoord + sampleDist * vec2(-0.203345,  0.620716);"
"   samples[4]  = vTexCoord + sampleDist * vec2(0.962340, -0.194983);"
"   samples[5]  = vTexCoord + sampleDist * vec2(0.473434, -0.480026);"
"   samples[6]  = vTexCoord + sampleDist * vec2(0.519456,  0.767022);"
"   samples[7]  = vTexCoord + sampleDist * vec2(0.185461, -0.893124);"
"   samples[8]  = vTexCoord + sampleDist * vec2(0.507431,  0.064425);"
"   samples[9]  = vTexCoord + sampleDist * vec2(0.896420,  0.412458);"
"   samples[10]  = vTexCoord + sampleDist * vec2(-0.321940, -0.932615);"
"   samples[11]  = vTexCoord + sampleDist * vec2(-0.791559, -0.597705);"
"   vec4 sample = texture2D(texture[0], vTexCoord);"
"   vec4 avg = sample;"
"   vec4 col;"
"   for (int i = 0; i < 12; i++)"
"   {"
"      vec2 tC=min(vec2(0.999,0.999),max(vec2(0.001,0.001),samples[i]));"
"      avg +=texture2D(texture[0],tC);"
"   }"
"   avg /= 13.0;"
"   avg.r=pow(avg.r,strength);"
"   avg.g=pow(avg.g,strength);"
"   avg.b=pow(avg.b,strength);"
"   gl_FragColor = sample+avg*multiplier;"
"}";

video::IVideoDriver* driver = smgr->getVideoDriver();
video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      callback= new PPE_Bloom_callback();
      callback->sampleDist=data.sampleDist;
      callback->strength=data.strength;
      callback->multiplier=data.multiplier;

      Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
        (
               vertShader.c_str(), "main", video::EVST_VS_1_1,
               fragShader.c_str(), "main", video::EPST_PS_1_1,
               callback, (video::EMT_SOLID)
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


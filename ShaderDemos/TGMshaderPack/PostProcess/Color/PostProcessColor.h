#ifndef __POST_PROCESS_EFFECT_COLOR_SATURATION__
#define __POST_PROCESS_EFFECT_COLOR_SATURATION__
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct PPE_Color_Setup
{
    float saturation;
    float coloringStrength;
    video::SColorf col;
};


class PPE_Color_callback: public video::IShaderConstantSetCallBack
{
public:
    float saturation;
    float coloringStrength;
    video::SColorf col;

  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    video::IVideoDriver* driver = services->getVideoDriver();

    services->setVertexShaderConstant("saturation", reinterpret_cast<f32*>(&saturation),1);
    services->setVertexShaderConstant("coloringStrength", reinterpret_cast<f32*>(&coloringStrength),1);
    services->setPixelShaderConstant("coloring", reinterpret_cast<f32*>(&col), 4);

	}
};




class IPostProcessColor : public scene::ISceneNode
{

public:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[6];//the vertices for the onscreenquad
   video::SMaterial Material;
   video::ITexture* rt0; //the rendertarget
   int mat;
   PPE_Color_callback* callback;

      IPostProcessColor(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id)
        {
      Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
      Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
      Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
      Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
        }


    void initiate(unsigned int sizeW,unsigned int sizeH,PPE_Color_Setup data,scene::ISceneManager* smgr)
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
"uniform float saturation;"
"uniform float coloringStrength;"
"uniform vec4  coloring;"

"void main()"
"{"
"   vec4 sample = texture2D(texture[0], vTexCoord);"
"   float sat=saturation/10.0;"
"   float hue=coloringStrength/10.0;"
"   float greyscale=sample.r*0.3+sample.g*0.49+sample.b*0.11;"
"   vec4 cGreyScale;"
"   cGreyScale.r=greyscale;"
"   cGreyScale.g=greyscale;"
"   cGreyScale.b=greyscale;"

"   sample=(sample*(1.0-hue))+coloring/255.0*greyscale*(hue);"
"   gl_FragColor = (cGreyScale*sat)+sample*(1.0-sat);"
"}";

video::IVideoDriver* driver = smgr->getVideoDriver();
video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      callback= new PPE_Color_callback;
      callback->saturation=data.saturation;
      callback->coloringStrength=data.coloringStrength;
      callback->col=data.col;

      Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
        (
               vertToonShader.c_str(), "main", video::EVST_VS_1_1,
               fragToonShader.c_str(), "main", video::EPST_PS_1_1,
               callback, (video::EMT_SOLID)
        );


      rt0 = driver->createRenderTargetTexture(core::dimension2d<s32>(sizeW,sizeH));
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


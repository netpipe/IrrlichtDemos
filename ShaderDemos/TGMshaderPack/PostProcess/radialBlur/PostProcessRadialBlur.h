#ifndef __POST_PROCESS_EFFECT_RADIAL_BLUR__
#define __POST_PROCESS_EFFECT_RADIAL_BLUR__
#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class PPE_RadialBlur_callback: public video::IShaderConstantSetCallBack
{
public:
    float sampleDist;
    float sampleStrength;
  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    video::IVideoDriver* driver = services->getVideoDriver();
    services->setVertexShaderConstant("sampleDist", reinterpret_cast<f32*>(&sampleDist),1);
    services->setVertexShaderConstant("sampleStrength", reinterpret_cast<f32*>(&sampleStrength),1);
	}
};




class IPostProcessRadialBlur : public scene::ISceneNode
{
    public:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[6];//the vertices for the onscreenquad
   video::SMaterial Material;
   video::ITexture* rt0; //the rendertarget
   int mat;
   PPE_RadialBlur_callback* callback;

      IPostProcessRadialBlur(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id): scene::ISceneNode(parent, mgr, id)
        {
      Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[1] = video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 0.0f);
      Vertices[2] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
      Vertices[3] = video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 1.0f);
      Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 0.0f, 1.0f);
      Vertices[5] = video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, video::SColor(255,0,255,255), 1.0f, 0.0f);
        }


    void initiate(int sizeW,int sizeH,float sampleDist,float sampleStrength,scene::ISceneManager* smgr)
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
"uniform float sampleDist;"
"uniform float sampleStrength;"

"void main()"
"{"
"vec2 dir=vec2(0.5,0.5)-vTexCoord;"
"float dist=sqrt(dir.x*dir.x+dir.y*dir.y);"
"dir=normalize(dir);"
"   float samples [10];"
"   samples [0]=-0.08;"
"   samples [1]=-0.05;"
"   samples [2]=-0.03;"
"   samples [3]=-0.02;"
"   samples [4]=-0.01;"
"   samples [5]=0.01;"
"   samples [6]=0.02;"
"   samples [7]=0.03;"
"   samples [8]=0.05;"
"   samples [9]=0.08;"
"   vec4 sample = texture2D(texture[0], vTexCoord);"
"   vec4 avg=sample;"
"   vec4 col;"
"   for (int i = 0; i < 10; i++)"
"   {"
"      vec2 tC=min(vec2(0.999,0.999),max(vec2(0.001,0.001),vTexCoord+dir*samples[i]*sampleDist));"

"      col=texture2D(texture[0],tC);"
"      float intensity=col.r+col.g+col.b;"
"      avg += col;  "
"   }"
"   avg /= 11.0;"
" float t=dist*sampleStrength;"
"t=clamp(t,0.0,1.0);"
"   gl_FragColor = mix(sample,avg,t);"
"}";
video::IVideoDriver* driver = smgr->getVideoDriver();
video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      callback= new PPE_RadialBlur_callback;
      callback->sampleDist=sampleDist;
      callback->sampleStrength=sampleStrength;
      Material.MaterialType=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
        (
               vertToonShader.c_str(), "main", video::EVST_VS_1_1,
               fragToonShader.c_str(), "main", video::EPST_PS_1_1,
               callback, (video::EMT_SOLID)
        );


      rt0 = driver->addRenderTargetTexture(core::dimension2d<u32>(sizeW,sizeH));
      Material.Wireframe = false;
      Material.Lighting = false;
      Material.setTexture(0, rt0);
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


// --------------------------------------------------------------------------
// Lava Shader Aplication By TGM
// feel free to use this as you want!
// --------------------------------------------------------------------------

#include <irrlicht.h>
// --------------------------------------------------------------------------

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



IrrlichtDevice *device;


const stringc vertLavaShader =
"varying vec2 Texcoord;"
"varying vec4 Pos;"
"void main( void )"
"{"
"    gl_Position = ftransform();"
"    Pos=gl_Vertex;"
"    Texcoord    = gl_MultiTexCoord0.xy;"
""
"}"
;
// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragLavaShader =
"uniform sampler2D Texture1;"
"uniform sampler2D Texture2;"
"uniform float time;"
""
"varying vec2 Texcoord;"
"varying vec4 Pos;"
"void main( void )"
"{"
"vec4 noise = texture2D( Texture1,Texcoord);"
"vec2     T1=  Texcoord+vec2(1.5,-1.5)*time*0.02;"
"vec2     T2=  Texcoord+vec2(-0.5,2.0)*time*0.01;"
"T1.x+=(noise.x)*2.0;"
"T1.y+=(noise.y)*2.0;"
"T2.x-=(noise.y)*0.2;"
"T2.y+=(noise.z)*0.2;"
"float     p=  texture2D( Texture1,T1*2.0).a;"
"vec4 color =  texture2D( Texture2, T2*2.0);"
"vec4 temp= color*(vec4(p,p,p,p)*2.0)+(color*color-0.1);"
"if(temp.r>1.0){temp.bg+=clamp(temp.r-2.0,0.0,100.0);}"
"if(temp.g>1.0){temp.rb+=temp.g-1.0;}"
"if(temp.b>1.0){temp.rg+=temp.b-1.0;}"
"    gl_FragColor = temp;"
"}"
;

class Shader_Lava_callback: public video::IShaderConstantSetCallBack
{
public:
    float time;
  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    time+=0.005;
    video::IVideoDriver* driver = services->getVideoDriver();
    services->setVertexShaderConstant("time", reinterpret_cast<f32*>(&time),1);
    int var0=0;
    services->setPixelShaderConstant("Texture1", (float*)(&var0), 1);
    int var1=1;
    services->setPixelShaderConstant("Texture2", (float*)(&var1), 1);
	}
};


// --------------------------------------------------------------------------



int main()
{
   // Create device
   video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
   device = createDevice(driverType, dimension2di(1024, 768), 32, false, false, false, 0);
   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();
   // Set a window caption
   device->setWindowCaption(L"Lava Shader Demo - TGMs Shader Pack");

   // Create GLSL shaders
   video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
   s32 mtlLavaShader = video::EMT_SOLID; // Fallback material type
   Shader_Lava_callback *callback= new Shader_Lava_callback;

 mtlLavaShader = gpu->addHighLevelShaderMaterial(
               vertLavaShader.c_str(), "main", video::EVST_VS_1_1,
               fragLavaShader.c_str(), "main", video::EPST_PS_1_1,
               callback, video::EMT_SOLID);
   IAnimatedMesh* mesh = smgr->getMesh("../../media/plane.3DS");


   ISceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
   if (node)
   {
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setMaterialTexture( 0, driver->getTexture("../../media/cloud.tga") );
      node->setMaterialTexture( 1, driver->getTexture("../../media/lavatile.jpg") );
      node->setMaterialType((video::E_MATERIAL_TYPE)mtlLavaShader); // Override material type
   }

   // Add a viewing camera
   smgr->addCameraSceneNodeFPS();

   // Main rendering loop
   while(device->run())
   {

      driver->beginScene(true, true, SColor(0,0,0,0));
      smgr->drawAll();
      driver->endScene();
          device->sleep(25);
   }

   device->drop();

   // Done
   return 0;
}

// --------------------------------------------------------------------------

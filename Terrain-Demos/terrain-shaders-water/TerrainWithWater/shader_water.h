#ifndef _shader_h_internal_h_
#define _shader_h_internal_h_


#include <irrlicht.h>
//#include <windows.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


const stringc vertWaterShader =
"uniform mat4 mWorld;"
"uniform vec4 CamPos;"
"varying vec4 GlassPos;"
"varying vec3 WorldView;"
"varying vec2 TexCoord;"
"varying vec3 VNormal;"
""
"void main()"
"{"
""
"    TexCoord = gl_MultiTexCoord0.xy*vec2(gl_Vertex.x,gl_Vertex.z);"
"    GlassPos = ftransform();"
""
"    vec3 Norm = gl_Normal.xyz;"
"    VNormal = normalize(vec3(mWorld * vec4(Norm.xyz,0)).xyz);"
"    "
"    vec4 PosW = mWorld * gl_Vertex;"
"    WorldView = CamPos.xyz - PosW.xyz;"
"	"
"    gl_Position = GlassPos;"
"}"
"";


const stringc fragWaterShader =
"uniform sampler2D RTTexture;"
"uniform sampler2D NormalMap;"
""
"uniform float timer;"
"varying vec3 WorldView;"
"varying vec4 GlassPos;"
"varying vec2 TexCoord;"
"varying vec3 VNormal;"
""
""
"void main()"
"{"
"    vec4 projCoord = GlassPos / GlassPos.w;"
"    projCoord += vec4(1.0);"
"    projCoord *= 0.5;"
"    vec2 norTex = TexCoord.xy+vec2(timer,1-timer);"
"    vec3 Normal = texture2D(NormalMap,norTex).xyz;"

"    vec3 clr = Normal; "
"    vec2 TexOff = Normal.xy * 2.0;"
"    TexOff -= vec2(1.0,1.0); "
"    "
"    projCoord.xy += ((TexOff.xy - vec2(0.5,0.5)) / 50.0);"
"    projCoord.xy = clamp(projCoord.xy, 0.0001, 0.9999);"
"    "
"    vec4 refTex = texture2D(RTTexture,projCoord.xy);"
""
"    gl_FragColor = refTex;"
"    gl_FragColor.xyz *= 0.8;"
" "
"}";







class Shader_Water_callback: public video::IShaderConstantSetCallBack
{
public:
    matrix4 WorldMat;
	IrrlichtDevice* device;
	ISceneManager* smgr;
	IVideoDriver* driver;

  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
	WorldMat = driver->getTransform(ETS_WORLD);
	services->setVertexShaderConstant("mWorld", WorldMat.pointer(), 16);

	vector3df CamPos = smgr->getActiveCamera()->getPosition();
	services->setVertexShaderConstant("CamPos", &CamPos.X, 4);


	int TexAddress = 0;
	services->setPixelShaderConstant("RTTexture", (float*)(&TexAddress), 1);
	int TexAddress2 = 1;
	services->setPixelShaderConstant("NormalMap", (float*)(&TexAddress2), 1);

	float tmr = device->getTimer()->getTime() * 0.0001f;
    services->setPixelShaderConstant("timer",(float*)(&tmr),1);

  }
};
#endif

#ifndef _cshader_internal_h_
#define _cshader_internal_h_


#include <irrlicht.h>
//#include <windows.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


const stringc vertBumpShaderLS =
""
"uniform mat4 matWorldInverse;"
""
"varying vec2 Texcoord;"
"varying vec2 Texcoord2;"
"varying vec3 ViewDirection;"
"varying vec3 LightDirection1;"
""
"float getLengthSQR (vec3 vec)"
"{"
"return dot(vec,vec);"
"}"
""
"void main( void )"
"{"
""
"   mat4 LightTransform= gl_ModelViewMatrix;"
"   LightTransform=LightTransform*matWorldInverse;"
""
"   gl_Position = ftransform();"
"   Texcoord    = gl_MultiTexCoord0.xy;"
"   Texcoord2    = gl_MultiTexCoord1.xy;"
""
"   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;"
""
"   vec3 fvViewDirection  =  - fvObjectPosition.xyz;"
""
""
"   vec3 fvNormal         = gl_NormalMatrix * gl_Normal;"
""
"   vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);"
"   vec3 fvBinormal =cross(fvTangent,gl_Normal);  "
"   fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);"
"   fvBinormal=gl_NormalMatrix*fvBinormal;"
""
""
"   ViewDirection.x  = dot( fvTangent, fvViewDirection );"
"   ViewDirection.y  = dot( fvBinormal, fvViewDirection );"
"   ViewDirection.z  = dot( fvNormal, fvViewDirection );"
""
"}";


const stringc fragBumpShaderLS =
"uniform vec4 fvAmbient;"
"uniform vec4 fvLight1Color;"
"uniform float fSpecularPower;"
"uniform float fSpecularStrength;"

"uniform sampler2D baseMap;"
"uniform sampler2D lightMap;"
"uniform sampler2D bumpMap;"

"varying vec2 Texcoord;"
"varying vec2 Texcoord2;"
"varying vec3 ViewDirection;"

"void main( void )"
"{"
"   vec3 normal = normalize(texture2D(bumpMap,Texcoord.xy).xyz * 2.0 - 1.0);"
"	vec3 camera_dir = normalize(ViewDirection);"
"   vec4 base = texture2D(baseMap, Texcoord.xy );"

"	gl_FragColor = texture2D(lightMap, Texcoord2.xy)*(base + pow(clamp(dot(reflect(-camera_dir,normal),camera_dir),0.0,1.0),fSpecularPower) * fSpecularStrength);"
"}"
;



class ShaderLightmapSpecularCallback: public video::IShaderConstantSetCallBack
{
public:
    SColorf fvAmbient;
    float fSpecularPower;
    float fSpecularStrength;

  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    int var0=0;
    services->setPixelShaderConstant("baseMap", (float*)(&var0), 1); //the colormap
    int var1=1;
    services->setPixelShaderConstant("lightMap", (float*)(&var1), 1); //the normalmap
    int var2=2;
    services->setPixelShaderConstant("bumpMap", (float*)(&var2), 1); //the normalmap

    core::matrix4 invWorld = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    invWorld.makeInverse();
    services->setPixelShaderConstant("matWorldInverse", (float*)(&invWorld), 16);
    services->setPixelShaderConstant("fvAmbient", (float*)(&fvAmbient), 4);
    services->setPixelShaderConstant("fSpecularPower", (float*)(&fSpecularPower), 1);
    services->setPixelShaderConstant("fSpecularStrength", (float*)(&fSpecularStrength), 1);
   }
};


#endif

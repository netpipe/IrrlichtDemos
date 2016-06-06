#ifndef _s_h_
#define _s_h_


#include <irrlicht.h>
//#include <windows.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class Shader_Terrain_callback: public video::IShaderConstantSetCallBack
{
public:

  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {
    int var0=0; services->setPixelShaderConstant("tex1", (float*)(&var0), 1);
    int var1=1; services->setPixelShaderConstant("tex2", (float*)(&var1), 1);
    int var2=2; services->setPixelShaderConstant("tex3", (float*)(&var2), 1);
    int var3=3; services->setPixelShaderConstant("tex4", (float*)(&var3), 1);

   }
};


const stringc vertTerrainShader =
""
"uniform mat4 matWorldInverse;"
""
"varying vec2 Texcoord;"
""
"void main( void )"
"{"
""
""
"   gl_Position = ftransform();"
"   Texcoord    = gl_MultiTexCoord0.xy;"
""
"   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;"
"}";


const stringc fragTerrainShader =

"uniform sampler2D tex1;"
"uniform sampler2D tex2;"
"uniform sampler2D tex3;"
"uniform sampler2D tex4;"

"varying vec2 Texcoord;"

"void main( void )"
"{"
"   vec4 base = texture2D(tex2, Texcoord.xy * 25.0 );"
"   base = mix(base, texture2D(tex3, Texcoord.xy * 25.0 ), smoothstep(-0.1,0.3,texture2D(tex1, Texcoord.xy).r));"
"   base = mix(base, texture2D(tex4, Texcoord.xy * 25.0 ), smoothstep(0.3,0.7,texture2D(tex1, Texcoord.xy).r));"
"	gl_FragColor = base;"

"}"
;
#endif

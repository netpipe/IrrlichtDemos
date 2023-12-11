#include "ToonShading.h"
using namespace irr;
//! the licence is on EntityManager.h

namespace GameShader
{
    toonShader::toonShader(irr::IrrlichtDevice*dev) : device(dev)
    {
        lightPosition = irr::core::vector3df(0,5000,0);
        camera = device->getSceneManager()->getActiveCamera();
        sourceShader();

        s32 mtlToonShader = video::EMT_SOLID; // Fallback material type
        video::IGPUProgrammingServices* gpu = device->getVideoDriver()->getGPUProgrammingServices();
        if (gpu && device->getVideoDriver()->getDriverType() == video::EDT_OPENGL)
        {
            if (device->getVideoDriver()->queryFeature(video::EVDF_ARB_GLSL))
            {
                mtlToonShader = gpu->addHighLevelShaderMaterial(
                            ToonShaderGLSL.c_str(), "vertex", video::EVST_VS_1_1,
                            ToonShaderGLSL.c_str(), "pixel" , video::EPST_PS_1_1,
                            this, video::EMT_SOLID);
            }
            else    printf("GLSL 1.1 not supported\n");
        }
        else if(gpu && device->getVideoDriver()->getDriverType() == video::EDT_DIRECT3D9)
        {
            if(device->getVideoDriver()->queryFeature(video::EVDF_VERTEX_SHADER_2_0) && device->getVideoDriver()->queryFeature(video::EVDF_PIXEL_SHADER_2_0))
            {
                mtlToonShader = gpu->addHighLevelShaderMaterial(
                            ToonShaderHLSL.c_str(), "vertex", video::EVST_VS_2_0,
                            ToonShaderHLSL.c_str(), "pixel" , video::EPST_PS_2_0,
                            this, video::EMT_SOLID);
            }
            else    printf("HLSL 1.1 not supported\n");
        }
        else    printf("No Shader for this driver\n");
        material = ((video::E_MATERIAL_TYPE)mtlToonShader); // Override material type
    }
    void toonShader::sourceShader()
    {
        /*********************************************************************/
        /************************** GLSL SHADER ******************************/
        /*********************************************************************/
        ToonShaderGLSL =
            "varying vec3 Normal;"
            "varying vec3 CamPos;"
            "uniform sampler2D Texture0;"
            "uniform vec3 LightDir;"
            "varying vec4 mWorldViewProj;"
            ""
            "void vertex()"
            "{"
            "    gl_FrontColor = gl_Color;"
            "    Normal = gl_NormalMatrix * gl_Normal;"
            "    gl_TexCoord[0] = gl_MultiTexCoord0;"
            "    gl_Position = ftransform();"
            "}"
            "vec4 CelShading ( vec4 color )"
            "{"
            "    float Intensity = dot( LightDir , normalize(Normal) );"
            "    float factor = 1.0;"
            "    if ( Intensity < 0.5 ) factor = 0.5;"
            "    color *= vec4 ( factor, factor, factor, 1.0 );"
            "    return color;"
            "}"
            "void pixel()"
            "{"
            "    vec4 color = texture2D( Texture0 , vec2( gl_TexCoord[0] ) );"
            "    color = CelShading ( color );"
            "    gl_FragColor = color;"
            "}";
        /*********************************************************************/
        /*************************** HLSL SHADER *****************************/
        /*********************************************************************/
        ToonShaderHLSL =
            "float4x4 mWorldViewProj;"
            "float3 LightDir;"
            "float3 CamPos;"
            "struct VS_OUTPUT"
            "{"
            "   float4 Position: POSITION0;"
            "   float2 TexCoord: TEXCOORD0;"
            "   float3 Normal: 	TEXCOORD1;"
            "   float3 LightDir: TEXCOORD2;"
            "   float3 EyeDir: TEXCOORD3;"
            "};"
            "VS_OUTPUT vertex(float4 Position: POSITION0,float2 TexCoord: TEXCOORD0,float2 diffuse: TEXCOORD1,float3 Normal:   NORMAL0)"
            "{"
            "   VS_OUTPUT Output;"
            "       Output.Position = mul(Position, mWorldViewProj);"
            "       Output.Normal = normalize(Normal);"
            "       Output.LightDir = normalize(LightDir - Position.xyz);"
            "       Output.EyeDir = normalize(CamPos - Position.xyz);"
            "       Output.TexCoord = TexCoord;"
            "   return(Output);"
            "}"
            "sampler2D tex0 : register(s0);"
            "sampler1D diffuseRamp : register(s1);"
            "float4 pixel"
            "("
            "   float2 TexCoord:     TEXCOORD0,"
            "   float3 Normal:       TEXCOORD1,"
            "   float3 LightDir:     TEXCOORD2,"
            "   float3 EyeDir:     	TEXCOORD3"
            ") : COLOR0"
            "{"
            "        float4 texCol = tex2D(tex0, TexCoord);"
            "        float diffuse = clamp(dot(Normal, LightDir), 0.0, 1.0);"
            "        float3 reflect = (2.0 * diffuse * Normal) - LightDir;"
            "        float specular = pow(clamp(dot(reflect, EyeDir), 0.0, 1.0), 8.0);"
            "            specular = clamp((specular - 0.5) * 1000.0, 0.0, 1.0);"
            "            float4 specular4 = specular * float4(1.0, 1.0, 1.0, 1.0);"
            "        float fresnal = dot(Normal, EyeDir);"
            "        float4 lightColor = tex1D(diffuseRamp, diffuse);"
            "        fresnal = clamp((fresnal - 0.2) * 1000.0, 0.0, 1.0);"
            "            lightColor *= fresnal;"
            "    return(saturate(lightColor * texCol + specular4));"
            "}";
        /*********************************************************************/
        /*********************************************************************/
        /*********************************************************************/
    }
}

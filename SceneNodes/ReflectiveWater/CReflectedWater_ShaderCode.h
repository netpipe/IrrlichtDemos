//
// Reflected Water.
// sio2 'at' users.sourceforge.net
// Based on .NET code by DeusXL (Irrlicht forum)
//

#ifndef _CREFLECTEDWATER_SHADERCODE_H
#define _CREFLECTEDWATER_SHADERCODE_H

const irr::c8 WATER_VERTEX_GLSL[] =
            "uniform float Time;\n" \
            "uniform float WaveHeight, WaveLength, WaveSpeed;\n" \
            "varying vec4 waterpos;\n" \
            "varying float addition;\n" \
            "void main()\n" \
            "{\n" \
            "   waterpos = ftransform();\n" \
            "   addition = (sin((gl_Vertex.x/WaveLength) + (Time * WaveSpeed / 10000.0))) +\n" \
            "              (cos((gl_Vertex.z/WaveLength) + (Time * WaveSpeed / 10000.0)));\n" \
                "   waterpos.y += addition * WaveHeight;\n" \
            "   gl_Position = waterpos;\n" \
            "}\n";
const irr::c8 WATER_FRAGMENT_GLSL[] =
            "uniform sampler2D ReflectionTexture;\n" \
            "uniform vec4 AddedColor, MultiColor;\n" \
                "uniform float UnderWater, WaveDisplacement, WaveRepetition, RefractionFactor;\n" \
            "varying vec4 waterpos;\n" \
            "varying float addition;\n" \
            "void main()\n" \
            "{\n" \
            "   vec4 projCoord = waterpos / waterpos.w;\n" \
            "   projCoord += vec4(1.0);\n" \
            "   projCoord *= 0.5;\n" \
            "   projCoord.x += sin(addition * WaveRepetition) * (WaveDisplacement / 1000.0);\n" \
            "   projCoord.y += cos(addition * WaveRepetition) * (WaveDisplacement / 1000.0);\n" \
            "   projCoord = clamp(projCoord, 0.001, 0.999);\n" \
            "   if(UnderWater == 0.0)\n" \
            "      projCoord.y = 1.0 - projCoord.y;\n" \
            "   vec4 refTex = texture2D(ReflectionTexture, vec2(projCoord));\n" \
            "   refTex = (refTex + AddedColor) * MultiColor;\n" \
                "   gl_FragColor = refTex;\n" \
                "   if(UnderWater == 1.0)\n" \
                "       gl_FragColor *= (MultiColor / 1.1);\n" \
                "   gl_FragColor.a = RefractionFactor;" \
            "}\n";
const irr::c8 WATER_HLSL[] =
                "uniform float Time;\n" \
                "float4x4 mWorldViewProj;\n" \
                "float WaveHeight, WaveLength, WaveSpeed;\n" \
                "float4 AddedColor, MultiColor;\n" \
                "float UnderWater, WaveDisplacement, WaveRepetition, RefractionFactor;\n" \
                "struct VS_OUTPUT\n" \
                "{\n" \
                "    float4 Position : POSITION;\n" \
                "    float4 Diffuse : COLOR0;\n" \
                "    float2 TexCoord : TEXCOORD0;\n" \
                "};\n" \
                "VS_OUTPUT vertexMain( in float4 vPosition : POSITION,\n" \
                "                      in float3 vNormal : NORMAL,\n" \
                "                      float2 texCoord : TEXCOORD0 )\n" \
                "{\n" \
                "    VS_OUTPUT Output;\n" \
                "    Output.Position = mul(vPosition, mWorldViewProj);\n" \
                "    float addition = (sin((vPosition.x/WaveLength) + (Time * WaveSpeed / 10000.0))) +\n" \
                "                     (cos((vPosition.z/WaveLength) + (Time * WaveSpeed / 10000.0)));\n" \
                "    Output.Position.y += addition * WaveHeight;\n" \
                "    Output.Diffuse = float4(addition, addition, addition, addition);\n" \
                "    Output.TexCoord = Output.Position / Output.Position.w;\n" \
                "    return Output;\n" \
                "}\n" \
                "struct PS_OUTPUT\n" \
                "{\n" \
                "    float4 RGBColor : COLOR0;\n" \
                "};\n" \
                "texture ReflectionTexture;\n" \
                "sampler MySampler = sampler_state\n" \
                "{\n" \
                "    Texture = ReflectionTexture;\n" \
                "    AddressU = CLAMP;\n" \
                "    AddressV = CLAMP;\n" \
                "};\n" \
                "PS_OUTPUT pixelMain( float2 TexCoord : TEXCOORD0,\n" \
                "                     float4 Position : POSITION,\n" \
                "                     float4 Diffuse : COLOR0 )\n" \
                "{\n" \
                "    PS_OUTPUT Output;\n" \
                "    float2 projCoord = TexCoord;\n" \
                "    float addition = Diffuse.r;\n" \
                "    projCoord += float2(1.0, 1.0);\n" \
                "    projCoord *= 0.5;\n" \
                "    projCoord.x += sin(addition * WaveRepetition) * (WaveDisplacement / 1000.0);\n" \
                "    projCoord.y += cos(addition *WaveRepetition) * (WaveDisplacement / 1000.0);\n" \
                "    if(UnderWater == 1.0)\n" \
                "        projCoord.y = 1.0 - projCoord.y;\n" \
                "    projCoord = clamp(projCoord, 0.001, 0.999);\n" \
                "    float4 refTex = tex2D(MySampler, projCoord);\n" \
                "    refTex = (refTex + AddedColor) * MultiColor;\n" \
                "    Output.RGBColor = refTex;\n" \
                "    if(UnderWater == 1.0)\n" \
                "        Output.RGBColor *= (MultiColor / 1.1);\n" \
                "    Output.RGBColor.a = RefractionFactor;\n" \
                "    return Output;\n" \
                "}";
const irr::c8 CLAMP_VERTEX_GLSL[] =
            "varying float cutoff;\n" \
            "void main()\n" \
            "{\n" \
            "   cutoff = gl_Vertex.y;\n" \
            "   gl_Position = ftransform();\n" \
            "   gl_TexCoord[0] = gl_MultiTexCoord0;\n" \
            "}\n";
const irr::c8 CLAMP_FRAGMENT_GLSL[] =
            "uniform sampler2D DiffuseMap, DetailMap;\n" \
                "uniform float WaterPositionY;\n" \
            "varying float cutoff;\n" \
            "void main()\n" \
            "{\n" \
            "   vec4 color = texture2D(DiffuseMap, gl_TexCoord[0].st) * 2.0 *\n" \
            "                texture2D(DetailMap, vec2(gl_TexCoord[0].x * 5.0, gl_TexCoord[0].y * 5.0));\n" \
                "   if(cutoff <= (WaterPositionY - 10.0))\n" \
            "      color.a = 0.0;\n" \
            "   else\n" \
            "      color.a = 1.0;\n" \
            "   gl_FragColor = color; \n" \
            "}\n";

const irr::c8 CLAMP_HLSL[] =
                "uniform float Time;\n" \
                "float4x4 mWorldViewProj;\n" \
                "float WaterPositionY;\n" \
                "struct VS_OUTPUT\n" \
                "{\n" \
                "    float4 Position : POSITION;\n" \
                "    float4 Diffuse : COLOR0;\n" \
                "    float2 TexCoord : TEXCOORD0;\n" \
                "    float2 TexCoord1 : TEXCOORD1;\n" \
                "};\n" \
                "VS_OUTPUT vertexMain( in float4 vPosition : POSITION,\n" \
             "                      in float3 vNormal : NORMAL,\n" \
             "                      float2 texCoord : TEXCOORD0,\n" \
             "                      float2 texCoord1 : TEXCOORD1)\n" \
                "{\n" \
                "    VS_OUTPUT Output;\n" \
                "    Output.Position = mul(vPosition, mWorldViewProj);\n" \
                "    Output.Diffuse = vPosition;\n" \
                "    Output.TexCoord = texCoord;\n" \
                "    Output.TexCoord1 = texCoord1;\n" \
                "    return Output;\n" \
                "}\n" \
                "struct PS_OUTPUT\n" \
                "{\n" \
                "    float4 RGBColor : COLOR0;\n" \
                "};\n" \
                "sampler2D DiffuseMap;\n" \
                "sampler2D DetailMap;\n" \
                "PS_OUTPUT pixelMain( float2 TexCoord : TEXCOORD0,\n" \
             "                     float2 TexCoord1 : TEXCOORD1,\n" \
             "                     float4 Position : POSITION,\n" \
             "                     float4 Diffuse : COLOR0 )\n" \
                "{\n" \
                "    PS_OUTPUT Output;\n" \
                "    float4 color = tex2D(DiffuseMap, TexCoord) * 2.0f *\n" \
              "                   tex2D(DetailMap, float2(TexCoord1.x * 5.0f, TexCoord1.y * 5.0f));\n" \
                "    if(Diffuse.y <= WaterPositionY)\n" \
                "        color.a = 0.0;\n" \
                "    else\n" \
                "        color.a = 1.0;\n" \
                "    Output.RGBColor = color;\n" \
                "    return Output;\n" \
                "}";

#endif


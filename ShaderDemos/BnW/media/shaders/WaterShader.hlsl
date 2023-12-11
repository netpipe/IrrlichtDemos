float4x4 mWorldViewProj;
float4x4 mWorld;
float4   CamPos;
float    Time;
float    sinWave;

struct VS_OUTPUT
{
  float4 Position   : POSITION0;
  float4 waterpos   : TEXCOORD0;
  float3 WorldView  : TEXCOORD1;
  float2 TexCoords  : TEXCOORD2;
  float3 MultiVar   : TEXCOORD3; // X: Addition, Y: Timer, Z: UnderWater
};

VS_OUTPUT vertexMain( float3 Position  : POSITION0,
                      float2 TexCoords : TEXCOORD0 )
{
  VS_OUTPUT OUT = (VS_OUTPUT)0;
  OUT.Position = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj);
  OUT.waterpos = float4(Position.x,Position.y,Position.z,1.0);

  OUT.MultiVar.y = Time / 10000.0;

  if (sinWave > 0.0)
  {
    OUT.MultiVar.x = (sin((Position.x/3.0) + (Time * 10.0 / 10000.0))) +
                     (cos((Position.z/3.0) + (Time * 10.0 / 10000.0)));
    OUT.Position.y += OUT.MultiVar.x;
  }

  OUT.MultiVar.z = CamPos.y;

  float4 PosW = mul(float4(Position.x,Position.y,Position.z,1.0), mWorld);
  OUT.WorldView = CamPos.xyz - PosW.xyz;

  OUT.TexCoords = TexCoords + float2(OUT.MultiVar.y, OUT.MultiVar.y);

  return(OUT);
}

// ----------------------------------------------------------------------------------------------------

float4x4 mWorldViewProjP;
float    sinWaveP;
float    refractionP;
float    seaLevel;

sampler2D ReflectionTexture : register(s0);
sampler2D NormalMap         : register(s1);
sampler2D DUDVMap           : register(s2);

float4 pixelMain( float4 waterpos   : TEXCOORD0,
                  float3 WorldView  : TEXCOORD1,
                  float2 TexCoord   : TEXCOORD2,
                  float3 MultiVar   : TEXCOORD3 // X: Addition, Y: Timer, Z: UnderWater
                ) : COLOR0
{
  float4 projCoord = mul(waterpos, mWorldViewProjP);
  projCoord.x = projCoord.x / projCoord.w / 2.0 + 0.5;
  projCoord.y = projCoord.y / projCoord.w / 2.0 + 0.5;

  if (sinWaveP > 0.0)
  {
    projCoord.x += sin(MultiVar.x * 5) * (2 / 1000.0);
    projCoord.y += cos(MultiVar.x * 5) * (2 / 1000.0);
    TexCoord.x  += sin(MultiVar.x * 5) * (5 / 1000.0);
    TexCoord.y  += cos(MultiVar.x * 5) * (5 / 1000.0);
  }

  if (refractionP > 0.0)
  {
    float4 DUDVoffset = tex2D(DUDVMap,TexCoord);
    projCoord.x += (DUDVoffset.x / 40.0) - (1.0 / 80.0);
    projCoord.y += (DUDVoffset.y / 40.0) - (1.0 / 80.0);
  }

  projCoord = clamp(projCoord, 0.001, 0.999);

  float4 normal = float4(0.0, 1.0, 0.0, 0.0);

  if(MultiVar.z < seaLevel)
    projCoord.y = 1.0 - projCoord.y;

  float4 refTex = tex2D(ReflectionTexture,projCoord);

  float facing = (1.0 - max(dot(normalize(WorldView), normalize(normal.xyz)), 0));
  float4 MultCol = float4(0.4,0.7,1.0,0.0);

  refTex = (refTex + float4(0.0,0.0,0.1,0.0)) * MultCol;

  float4 norMap =  tex2D(NormalMap,TexCoord);
  float lightComp = 1.0 - max(dot(normalize(norMap.xyz),normalize(abs(WorldView))),0.0);

  float4 finalCol = refTex;

  if(MultiVar.z < seaLevel)
    finalCol *= 0.8;

  finalCol.a = 0.6 + (facing / 2.0);
  finalCol += lightComp;

  return(finalCol);
};
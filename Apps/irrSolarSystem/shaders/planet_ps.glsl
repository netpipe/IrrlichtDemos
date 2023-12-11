//Lights
float4 Light1_C; //float3 Light1_P; float Light1_Range;
float4 Light_Ambient;

//Material
float spec_factor;
float spec_power;
sampler Texture0 : register(s0);

float Light_Spec(float3 Normal,
                 float3 HalfVec)
{
   float SpecAttn = pow(clamp(0, 1, dot(Normal, HalfVec)),pow(2,spec_factor*10));
   return SpecAttn*spec_power;
}

float4 Light_Point(float3 LightDir,
                   float3 Normal,
                   float4 LightColor,
                   float3 HalfVec,
                   float2 TexCoord)
{
   // Compute suface/light angle based attenuation defined as dot(N,L)
   float AngleAttn = clamp(0, 1, dot(Normal, LightDir));
   
   // Compute final lighting
   return clamp(LightColor * AngleAttn,0,1);
}

float4 ps_main(float2 inTexCoord:TEXCOORD0,
               float3 inNormal:TEXCOORD1,
               float3 inLightDir1:TEXCOORD2,
               float3 inHalfVec1:TEXCOORD3) : COLOR
{
   float SpecAttn = Light_Spec(inNormal,inHalfVec1);

   // Compute the lighting contribution for this single light
   return tex2D(Texture0,inTexCoord)
            *(
            /*Light1*/Light_Point(inLightDir1,inNormal,Light1_C, inHalfVec1, inTexCoord)+SpecAttn*Light1_C+
            /*Ambient*/Light_Ambient
            );
}
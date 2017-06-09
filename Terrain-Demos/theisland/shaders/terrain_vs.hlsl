float4x4 mWorldViewProj : WorldViewProjection;
float4x4 mWorld : World; 

float3 mLightDir : Direction = float3(0,1,1); 

sampler2D splat1; 
sampler2D splat2; 

struct VS_IN
{
   float4 Position    : POSITION;
   float3 Normal      : NORMAL;
   float4 Diffuse     : COLOR0;
   float2 TexCoord0   : TEXCOORD0;
};
struct VS_OUT
{
   float4 Position   : POSITION;   // vertex position 
   float4 Diffuse    : COLOR0;     // vertex diffuse color 
    
   float2 SplatTexCoord : TEXCOORD1; //1st spalt texture coords 
   float3 LightDir		 : TEXCOORD2;
   float3 Normal : TEXCOORD3;    
   float normalDist : TEXCOORD0;
};
struct PS_OUT
{
	float4 Color : COLOR0;  // Pixel color
};

VS_OUT vs_main(VS_IN Input)
{
	VS_OUT Output = (VS_OUT)0;
	Output.Position = mul(Input.Position, mWorldViewProj);
	Output.LightDir = mLightDir;
	Output.Normal = mul(Input.Normal,mWorld);
	Output.SplatTexCoord = Input.TexCoord0;
	if (Input.Position.y > 268) {
		Output.normalDist = Input.Normal.y/2;
	}else{
		Output.normalDist = Input.Normal.y;
	}
	return Output;
}

PS_OUT ps_main(VS_OUT Input)
{
	PS_OUT Output = (PS_OUT)0;
	float3 lightDir = normalize(Input.LightDir);
    float4 TexColor = lerp(tex2D( splat1, Input.SplatTexCoord),tex2D( splat2, Input.SplatTexCoord),(Input.normalDist));
    float LightColor = saturate(dot(lightDir,Input.Normal));
	Output.Color = TexColor * LightColor;
	return Output;
}

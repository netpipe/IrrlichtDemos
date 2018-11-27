float4x4 mWorldViewProj;
float4x4 mWorldViewProj2;
float3 LightPos;
float ShadDark;
float MaxD;
float EnableLighting;
float MAPRES;

struct VS_OUTPUT 
{
	float4 Position				: POSITION0;
	float4 ShadowMapSamplingPos : TEXCOORD0; 
	float4 MVar        			: TEXCOORD1;
	float2 TexCoords            : TEXCOORD2;
};

VS_OUTPUT vertexMain( 
   	float3 Position	: POSITION0,
	float2 TexCoords : TEXCOORD0,
	float2 TexCoords2 : TEXCOORD1,
	float3 Normal : NORMAL
  )
{
	VS_OUTPUT  OUT;
    OUT.Position = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj);
	float4 SMPos = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj2);
	SMPos.xy = float2(SMPos.x, -SMPos.y) / 2.0;
	
	OUT.ShadowMapSamplingPos = SMPos;
		
	float3 LightDir = normalize(LightPos - Position.xyz);
	
	OUT.MVar = float4(SMPos.z, dot(Normal, LightDir), 1.0 / MAPRES, MaxD);
	OUT.TexCoords = TexCoords;
	
	return(OUT);
}

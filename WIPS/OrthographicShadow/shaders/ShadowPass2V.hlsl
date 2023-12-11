float4x4 mWorldViewProj;  // World * View * Projection transformation
float4x4 mWorldViewProj2;
float3 LightPos;
float ShadDark;
float MaxD;
float EnableLighting;

struct VS_OUTPUT 
{
	float4 Position				: POSITION0;
	float4 ShadowMapSamplingPos : TEXCOORD0; 
	float4 MVar        			: TEXCOORD1;
	float2 TexCoords            : TEXCOORD2;
	float4 ColPass 				: TEXCOORD3;
	float2 TexCoords2           : TEXCOORD4;
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
	OUT.MVar[0] = SMPos.z;

	OUT.ShadowMapSamplingPos = SMPos;
		
	if(EnableLighting > 0.5f)
	{
		float3 LightDir = normalize(LightPos - Position.xyz);
		OUT.MVar[1] = dot(Normal,LightDir);
	}
	else
	{
		OUT.MVar[1] = 1.0f;
	}

	OUT.MVar[2] = ShadDark / (SAMPLE_AMOUNT * 2.0f);
	OUT.MVar[3] = MaxD;
	
	OUT.ColPass = float4(0,0,0,0);
	
	OUT.TexCoords = TexCoords;
	OUT.TexCoords2 = TexCoords2;
	
	return(OUT);
}

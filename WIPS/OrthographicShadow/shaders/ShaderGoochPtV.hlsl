//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 mWorldViewProj;  // World * View * Projection transformation
float4x4 mInvWorld;       // Inverted world matrix
float4x4 World;       // world matrix
float4x4 WorldI;
float4 LightPos;     



struct VS_OUTPUT 
{
   float4 Position: POSITION0;
   float4 Color: COLOR0;
};


VS_OUTPUT vertexMain( 
   	float4 Position	: POSITION0,
	float3 Normal	: NORMAL0
)
{
	VS_OUTPUT  OUT;
	float3 Nn = -Normal;
	float4 Po = float4(Position.xyz,1);
	//compute worldspace position
	float3 Pw = mul(Po, World).xyz;
	float3 Ln = normalize(Pw - LightPos).xyz;
	float mixer = 0.5 * (dot(Nn,Ln) + 1.0);
	float3 surfColor = lerp(float3(0.0f, 0.0f, 0.0f),float3(0.8f, 0.5f, 0.1f),mixer);
	float3 toneColor = lerp(float3(0.05f, 0.05f, 0.6f),float3(0.5f, 0.4f, 0.05f),mixer);
	float3 mixColor = surfColor + toneColor;
	OUT.Color = float4(mixColor.xyz,1.0f);
	OUT.Position =  mul(Po, mWorldViewProj);
	return(OUT);
}
	
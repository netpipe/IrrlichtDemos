float4x4 mWorldViewProj;
float MaxD;

struct VS_OUTPUT 
{
	float4 Position: POSITION0;
	float4 Color: TEXCOORD0;
	float2 Texcoords: TEXCOORD1;
};

VS_OUTPUT vertexMain(float3 Position : POSITION0, float2 Texcoords : TEXCOORD0)
{
	VS_OUTPUT  OUT;
	float4 hpos = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj);
    OUT.Color = hpos;
	OUT.Color.w = MaxD;
    OUT.Position = hpos;
	OUT.Texcoords = Texcoords;
	return(OUT);
}
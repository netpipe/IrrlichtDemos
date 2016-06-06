float4x4 mWorldViewProj;
float Timer;
float MaxD;

struct VS_OUTPUT 
{
   float4 Position: POSITION0;
   float4 UV	: TEXCOORD0;
};


VS_OUTPUT vertexMain(float3 Position : POSITION0)
{
	VS_OUTPUT  OUT;
	float timeNow = Timer / 4;
	float4 Po = float4(Position.xyz,1);
	float iny = Po.y * 0.5 + timeNow;
	float wiggleX = sin(iny) * 0.5;
	float wiggleY = cos(iny) * 0.5; // deriv
	Po.x = Po.x + wiggleX;
	OUT.Position = mul(Po, mWorldViewProj);
	OUT.UV = OUT.Position;
	OUT.UV.x = MaxD;
	return(OUT);
}

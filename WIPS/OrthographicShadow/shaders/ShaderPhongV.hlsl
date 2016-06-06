float4x4 mWorldViewProj;
float4x4 World;
float4 LightPos;
float4 CamPos;

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float3 Light : TEXCOORD0;
    float3 Norm : TEXCOORD1;
    float3 View : TEXCOORD2;
	float2 TexCo : TEXCOORD3;
};

VS_OUTPUT vertexMain(float4 Pos : POSITION, float3 Normal : NORMAL, float2 TexC : TEXCOORD0)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    Out.Pos = mul(Pos, mWorldViewProj); // transform Position
    Out.Light = normalize(LightPos - Pos.xyz);              // L
    float3 PosWorld = normalize(mul(Pos, World));
    Out.View = normalize(CamPos - PosWorld);         // V
    Out.Norm = normalize(Normal);     // N
	Out.TexCo = TexC;
	return Out;
}

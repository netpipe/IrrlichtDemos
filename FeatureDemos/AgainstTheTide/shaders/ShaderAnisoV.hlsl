//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 mWorldViewProj;  // World * View * Projection transformation
float4x4 World;       // world matrix
float4 LightPos;     
float4x4 ViewI;


struct VS_OUTPUT 
{
   float4 Position: POSITION0;
   float2 UV	: TEXCOORD0;
};


VS_OUTPUT vertexMain( 
   	float3 Position	: POSITION0,
    float4 Normal : NORMAL
 )
{
	VS_OUTPUT  OUT;
	float3 Nn = Normal.xyz;
    float4 Po = float4(Position.xyz,1.0);    // obj coords
    float3 Pw = mul(Po, World).xyz; // world coords
    float3 Vn = normalize(ViewI[3].xyz - Pw);
    float3 Ln = normalize(LightPos - Position.xyz);
    float3 Hn = normalize(Vn + Ln);
    OUT.UV = float2(dot(Ln, Nn), dot(Hn, Nn));
    OUT.Position = mul(Po, mWorldViewProj);
	return(OUT);
}

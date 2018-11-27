//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 mWorldViewProj;  // World * View * Projection transformation
float4x4 mInvWorld;       // Inverted world matrix
float4x4 World;       // world matrix
float4 LightPos;     
float Timer;
float4x4 ViewI;


struct VS_OUTPUT 
{
   float4 Position: POSITION0;
   float2 UV	: TEXCOORD0;
   float4 Color: COLOR0;
   float4 specCol	: COLOR1;
};


VS_OUTPUT vertexMain( 
   	float3 Position	: POSITION0,
    float4 UV		: TEXCOORD0,
    float4 Tangent	: TANGENT0,
    float4 Binormal	: BINORMAL0,
    float4 Normal	: NORMAL0
)
{
	VS_OUTPUT  OUT;
	float3 Nn = normalize(mul(Normal, mInvWorld).xyz);
    float timeNow = Timer / 4;
    float4 Po = float4(Position.xyz,1);
    float iny = Po.y * 0.5 + timeNow;
    float wiggleX = sin(iny) * 1.5;
    float wiggleY = cos(iny) * 1.5;
    Nn.y = Nn.y + wiggleY;
    Nn = normalize(Nn);
    Po.x = Po.x + wiggleX;
    OUT.Position = mul(Po, mWorldViewProj);
    float3 Pw = mul(Po, World).xyz;
    float3 Ln = normalize(LightPos - Pw);
    float ldn = dot(Ln,Nn);
    float diffComp = max(0,ldn);
    float3 diffContrib = float3(0.9f, 0.68f, 0.543f) * ( diffComp * float3(1.0f, 1.0f, 1.0f) + float3(0.2f, 0.2f, 0.2f));
    OUT.Color = float4(diffContrib,1);
    OUT.UV = UV;
    float3 Vn = normalize(ViewI[3].xyz - Pw);
    float3 Hn = normalize(Vn + Ln);
    float hdn = pow(max(0,dot(Hn,Nn)),5.0);
    OUT.specCol = float4(hdn * float3(1.0f, 1.0f, 1.0f),1);
	return(OUT);
}
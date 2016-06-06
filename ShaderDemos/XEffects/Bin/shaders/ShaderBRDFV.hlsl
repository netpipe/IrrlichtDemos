//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 mWorldViewProj;  // World * View * Projection transformation
float4x4 World;       // world matrix
float4 CamPos; 
float4 LightPos;  
  



struct VS_OUTPUT 
{
    float4 HPosition	: POSITION;
    float2 HalfAngleTerms	: TEXCOORD0; // dot prods against half-angle
    float2 NormalAngleTerms	: TEXCOORD1; // dot-prods against normal
    float2 UV	: TEXCOORD2; // plain-ol' UV
    float4 diffCol	: COLOR0;
};


VS_OUTPUT vertexMain( 
   	float3 Position	: POSITION0,
    float4 UV		: TEXCOORD0,
    float4 Normal	: NORMAL0
)
{
	VS_OUTPUT  OUT;
	 float3 Nn = -Normal.xyz;	// worldspace normal
    float4 Po = float4(Position.xyz,1);				// object space pos
    float3 Pw = mul(Po, World).xyz;						// worldspace pos
    float3 Ln = normalize(LightPos - Pw);				// worldspace Light vector
    float ldn = dot(Ln,Nn);
    float diffComp = max(0,ldn);
    OUT.diffCol = float4(0.5f, 0.2f, 0.1f, 1.0f) * float4(0.1f, 0.1f, 0.1f, 1.0f);
    OUT.UV = UV.xy;
    float3 EyePos = CamPos[3];						// worldspace eye position
    float3 Vn = normalize(EyePos - Pw);
    float3 Hn = normalize(Vn + Ln);
    float2 halfIndices = float2(0.5+dot(Ln,Hn)/2.0,
								1.0 - (0.5+dot(Nn,Hn)/2.0));
    float2 normIndices = float2(0.5+dot(Ln,Nn)/2.0,
								1.0 - (0.5+dot(Nn,Vn)/2.0));
    OUT.HalfAngleTerms = halfIndices;
    OUT.NormalAngleTerms = normIndices;
    // transform into homogeneous-clip space
    OUT.HPosition = mul(Po, mWorldViewProj);
	return(OUT);
}
	
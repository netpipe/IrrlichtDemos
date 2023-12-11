#define MAX_JOINT_NUM 55
#define MAX_LIGHT_NUM 8

float4x4 JointTransform[MAX_JOINT_NUM];
float4x4 mWorldViewProj;
float4x4 mWorld;

float3 lightPosArray[MAX_LIGHT_NUM];
float4 lightColorArray[MAX_LIGHT_NUM];

struct VS_OUTPUT 
{
	float4 Position		: POSITION0;
	float2 TexCoords 	: TEXCOORD0; 
	float2 TexCoords2 	: TEXCOORD1;
	float4 Color : COLOR0;
};

VS_OUTPUT vertexMain
( 
   	float3 Position		: POSITION0,
	float2 TexCoords 	: TEXCOORD0,
	float2 TexCoords2 	: TEXCOORD1,
	float4 Color : COLOR0,
	float3 Normal : NORMAL

)
{
	VS_OUTPUT  OUT;
	
    float4 MeshPos = float4(Position.x,Position.y,Position.z,1.0);
	float4x4 ModelTransform = mWorldViewProj;
	
	int verCol = int(Color.r * 255.9);
	
	float4x4 matTran = JointTransform[verCol - 1];
	
	verCol = int(Color.g * 255.9);
	
	if(verCol != 0)
		matTran += JointTransform[verCol - 1];
		
	verCol = int(Color.b * 255.9);
	
	if(verCol != 0)
		matTran += JointTransform[verCol - 1];
		
	verCol = int(Color.a * 255.9);
	
	if(verCol != 0)
		matTran += JointTransform[verCol - 1];
	
	// Apply final transformation.
	ModelTransform = mul(matTran, ModelTransform);
	
	OUT.Position = mul(MeshPos, ModelTransform);
	OUT.TexCoords = TexCoords;
	OUT.TexCoords2 = TexCoords2;
	
	float4 finalCol = float4(0,0,0,0);
	float4 Pos = mul(float4(Position.x,Position.y,Position.z,1.0), mWorld);
	
	for(int i = 0;i < MAX_LIGHT_NUM;++i)
	{
			float3 lightDirection = normalize(lightPosArray[i] - Pos);
			finalCol +=	lightColorArray[i] * dot(Normal,lightDirection);
	}
	
	OUT.Color = clamp(finalCol,0.3,1.0);
	
	return(OUT);
}
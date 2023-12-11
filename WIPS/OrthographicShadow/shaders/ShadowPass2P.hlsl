
sampler2D ColoredTextureSampler : register(s0);

##ifdef 2_LAYER
	sampler2D DetailMapSampler : register(s1);
	sampler2D ShadowMapSampler : register(s2);
##else
	sampler2D ShadowMapSampler : register(s1);
##endif

float calcShadow(float2 texCoords, float2 offset, float RealDist, float DarkNess)
{
	// Unpack 16 bit info.
	float4 shadTexCol = tex2D(ShadowMapSampler,texCoords + offset);
	float extractedDistance = (shadTexCol.r + (shadTexCol.g / 256.0f));
		
	return (extractedDistance <= RealDist ? DarkNess : 0.0f);
};

float4 pixelMain(
	float4 SMPos 			: TEXCOORD0,
	float4 MVar     : TEXCOORD1,
	float2 TexCoords       	: TEXCOORD2,
	float4 ColPass : TEXCOORD3
##ifdef 2_LAYER
	,float2 TexCoords2      : TEXCOORD4
##endif
	) : COLOR0
{

const float2 offsetArray[16] = 
{
	float2(0,0),
	float2(0,1.0 / MAPRES),
	float2(1.0 / MAPRES,-1.0 / MAPRES),
	float2(-1.0 / MAPRES,1.0 / MAPRES),
	float2(-2.0 / MAPRES,0),
	float2(0,-2.0 / MAPRES),
	float2(-2.0 / MAPRES,-2.0 / MAPRES),
	float2(2.0 / MAPRES,2.0 / MAPRES),
	float2(3.0 / MAPRES,0),
	float2(0,3.0 / MAPRES),
	float2(3.0 / MAPRES,-3.0 / MAPRES),
	float2(-3.0 / MAPRES,3.0 / MAPRES),
	float2(-4.0 / MAPRES,0),
	float2(0,-4.0 / MAPRES),
	float2(-4.0 / MAPRES,-4.0 / MAPRES),
	float2(4.0 / MAPRES,4.0 / MAPRES),
};

SMPos.x = SMPos.x / SMPos.w / 2.0f + 0.5f;
SMPos.y = -SMPos.y / SMPos.w / 2.0f + 0.5f;

MVar[0] /= MVar[3];
MVar[0] -= 0.01f;

##ifdef 2_LAYER
float4 finalCol = tex2D(ColoredTextureSampler, TexCoords) * tex2D(DetailMapSampler, TexCoords2);
##else
float4 finalCol = tex2D(ColoredTextureSampler, TexCoords);
##endif

if(((saturate(SMPos.x) == SMPos.x) && (saturate(SMPos.y) == SMPos.y)) && SMPos.z > 0.0 && SMPos.z < MVar[3])
	for(int i = 0;i < SAMPLE_AMOUNT; ++i)
		MVar[1] -= calcShadow(SMPos.xy, offsetArray[i],MVar[0],MVar[2]);
  
return(finalCol * MVar[1]);	
}
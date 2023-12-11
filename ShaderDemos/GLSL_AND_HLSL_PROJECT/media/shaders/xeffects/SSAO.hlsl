sampler2D DepthMapSampler : register(s2);
sampler2D UserMapSampler : register(s3);

float4x4 mViewProj;

float getDepthAt(float2 coords)
{
	float4 texDepth = tex2D(DepthMapSampler, coords);
	float extractedDepth = texDepth.r;
	return extractedDepth;
}

#define SAMPLE_COUNT 8
#define SSAO_REACH 1.0
#define MAX_DEPTH_DECLINE 0.2
#define DEPTH_ALIASING_EPISILON 0.02
#define RAND_TEXTURE_TILES 3.0
#define SSAO_OUTPUT_MULTIPLIER 1.5

float4 pixelMain
(
	float2 TexCoords : TEXCOORD0,
	float3 LStart : TEXCOORD1,
	float3 LEnd : TEXCOORD2
) : COLOR0
{
	float currentDepth = getDepthAt(TexCoords);
	float3 lVec = LEnd - LStart;
	float3 lDir = normalize(lVec);
	float lLength = length(lVec);
	float3 currentWPos = LStart + currentDepth * lDir * lLength;
	
	const float3 offsets[SAMPLE_COUNT] =
	{
		float3( 0.3524245,  0.2452452,  0.7425442),
		float3(-0.5242424, -0.4125105, -0.7867286),
		float3(-0.7857242,  0.2453452,  0.7242454),
		float3(-0.1524532, -0.9456342, -0.1452245),
		float3(-0.2535434,  0.4535424,  0.5444254),
		float3( 0.3135244, -0.4330132, -0.5422425),
		float3( 0.7897852, -0.2524242,  0.4584555),
		float3( 0.9792424,  0.0122242, -0.5422161)
	};
	
	float totalOcclusion = 0.0;
	float4 randCol = tex2D(UserMapSampler, TexCoords * RAND_TEXTURE_TILES);

	for(int i = 0;i < SAMPLE_COUNT;++i)
	{
		// Reflect the random offset using the random user texture.
		float3 randVec = reflect(normalize(offsets[i]), randCol.xyz * 2.0 - float3(1.0, 1.0, 1.0)) * SSAO_REACH;
		float4 newWorldPos = float4(currentWPos + randVec, 1.0);
		
		// Calculate our sample depth.
		float sampleDepth = (length(newWorldPos.xyz - LStart) / lLength) - DEPTH_ALIASING_EPISILON;
	
		// Project the position to the screen.
		newWorldPos = mul(newWorldPos, mViewProj);
		newWorldPos.xy = float2(newWorldPos.x, -newWorldPos.y) / newWorldPos.w * 0.5 + float2(0.5, 0.5);

		// Read from the projected position.
		float newDepth = getDepthAt(clamp(newWorldPos.xy, float2(0.001, 0.001), float2(0.999, 0.999)));

		// Compare the two depth samples.
		float depthCalc = newDepth < sampleDepth ? 1.0 : 0.0;
		depthCalc *= clamp(1.0 - (sampleDepth - newDepth) / MAX_DEPTH_DECLINE, 0.0, 1.0);
		totalOcclusion += depthCalc;
	}
	
	totalOcclusion /= SAMPLE_COUNT;
	
	return 1.0 - totalOcclusion * SSAO_OUTPUT_MULTIPLIER;
}
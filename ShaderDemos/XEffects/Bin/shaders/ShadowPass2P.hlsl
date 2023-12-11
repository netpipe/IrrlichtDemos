sampler2D ShadowMapSampler : register(s0);
float4 LightColour;

float calcShadow(float2 texCoords, float2 offset, float RealDist)
{
   // Unpack 16 bit info.
   float4 shadTexCol = tex2D(ShadowMapSampler,texCoords + offset);
   float extractedDistance = (shadTexCol.r + (shadTexCol.g / 256.0));
      
   return (extractedDistance <= RealDist ? (1.0  / SAMPLE_AMOUNT) : 0.0);
};

float4 pixelMain
(
   float4 SMPos       : TEXCOORD0,
   float4 MVar        : TEXCOORD1,
   float2 TexCoords    : TEXCOORD2
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

	SMPos.xy = float2(SMPos.x, -SMPos.y) / SMPos.w / 2.0f + float2(0.5, 0.5);

	float4 finalCol = float4(0.0, 0.0, 0.0, 0.0);
	float2 clampedSMPos = saturate(SMPos.xy);
	float lightFactor = 1.0;
	float realDistance = MVar[0] / MVar[3] - 0.005;

	// If this point is within the light's frustum.
	if(((clampedSMPos.x == SMPos.x) && (clampedSMPos.y == SMPos.y)) && SMPos.z > 0.0 && SMPos.z < MVar[3])
	{
		for(int i = 0;i < SAMPLE_AMOUNT; ++i)
			lightFactor -= calcShadow(SMPos.xy, offsetArray[i], realDistance);

		// Multiply with diffuse.
		finalCol = LightColour * lightFactor * MVar[1];
	}
	
	return finalCol;
}
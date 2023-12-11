sampler2D ScreenMapSampler : register(s1);
sampler2D DepthMapSampler : register(s2);
sampler2D UserMapSampler : register(s3);

float getDepthAt(float2 coords)
{
	float4 texDepth = tex2D(DepthMapSampler, coords);
	return texDepth.r;
}

#define TEST_EPISILON 0.5

#define WATER_COLOR float4(0.2, 0.8, 1.0, 1.0);
#define SPECULAR_COLOR float4(0.6, 0.6, 0.6, 0.6)
#define SPECULAR_EXPONENT 32.0

float4 pixelMain
(
	float2 TexCoords : TEXCOORD0,
	float3 LStart : TEXCOORD1,
	float3 LEnd : TEXCOORD2
) : COLOR0
{
	float3 lVec = LEnd - LStart;
	float3 lDir = normalize(lVec);
	float lLength = length(lVec);
	float currentDepth = getDepthAt(TexCoords) * lLength;
	float3 currentWPos = LStart + currentDepth * lDir;
	
	float4 screenColor = tex2D(ScreenMapSampler, TexCoords.xy);
	
	float3 planeNormal = float3(0.0, LStart.y < 0.0 ? -1.0 : 1.0, 0.0);
	float planeDP = dot(planeNormal, lDir);

	if(LStart.y < 0.0)
		screenColor *= WATER_COLOR;
	
	float4 finalColour = screenColor;

	if(planeDP < 0.0)
	{
		float planeDepth = -dot(planeNormal, LStart) / planeDP;

		if(planeDepth < currentDepth && planeDepth < lLength)
		{
			float3 planePos = LStart + lDir * planeDepth;
		
			float heightDiff = sin(planePos.x / 20.0) + sin(planePos.z / 20.0);
			planePos.y += heightDiff;

			float4 normalMap = tex2D(UserMapSampler, planePos.xz / 50.0);
			float3 lightNormal = normalMap.xyz - float3(0.5, 0.5, 0.5);
			lightNormal = normalize(lightNormal);
			float2 refCoords = clamp(TexCoords.xy + lightNormal.xy / 20.0, float2(0.01, 0.01), float2(0.99, 0.99));
			float refDepth = getDepthAt(refCoords) * lLength;
			
			finalColour = WATER_COLOR;

			if(refDepth - TEST_EPISILON > planeDepth)
				finalColour *= tex2D(ScreenMapSampler, refCoords);
			else
				finalColour *= screenColor;

			float3 sunDirection = float3(1.0, 1.0, 1.0);
			float diffuse = dot(lightNormal, sunDirection);
			float3 Reflect = normalize(2.0 * diffuse * lightNormal - sunDirection);
			float4 specular = pow(saturate(dot(Reflect, lDir)), SPECULAR_EXPONENT) * SPECULAR_COLOR;
			finalColour += specular;

			//float4 foamColour = float4(0.8, 0.8, 0.8, 0.8) * diffuse;			
			//float foamFac = clamp(((currentDepth - planeDepth - 3.0) / 5.0), 0.0, 1.0);
			//finalColour = lerp(finalColour + foamColour, finalColour, foamFac);

			float shoreFac = clamp((currentDepth - planeDepth) / 20.0, 0.0, 1.0);

			finalColour = lerp(screenColor, finalColour, shoreFac);
			
			float fadeFac = clamp(1.0 - (planeDepth - lLength / 10.0) / lLength, 0.0, 1.0);
			finalColour = lerp(screenColor, finalColour, fadeFac);
		}
	}
	
	return finalColour;
}
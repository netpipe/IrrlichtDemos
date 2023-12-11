uniform sampler2D DepthMapSampler;
uniform sampler2D UserMapSampler;

uniform mat4 mViewProj;

float getDepthAt(vec2 coords)
{
	vec4 texDepth = texture2D(DepthMapSampler, coords);
	float extractedDepth = texDepth.r;
	return extractedDepth;
}

#define SAMPLE_COUNT 8
#define SSAO_REACH 1.0
#define MAX_DEPTH_DECLINE 0.2
#define DEPTH_ALIASING_EPISILON 0.02
#define RAND_TEXTURE_TILES 3.0
#define SSAO_OUTPUT_MULTIPLIER 1.5

vec3 offsets[SAMPLE_COUNT];

void main()
{
	float currentDepth = getDepthAt(gl_TexCoord[0].xy);
	vec3 lVec = gl_TexCoord[2].xyz - gl_TexCoord[1].xyz;
	vec3 lDir = normalize(lVec);
	float lLength = length(lVec);
	vec3 currentWPos = gl_TexCoord[1].xyz + currentDepth * lDir * lLength;
	
	offsets[0] = vec3( 0.3524245,  0.2452452,  0.7425442);
	offsets[1] = vec3(-0.5242424, -0.4125105, -0.7867286);
	offsets[2] = vec3(-0.7857242,  0.2453452,  0.7242454);
	offsets[3] = vec3(-0.1524532, -0.9456342, -0.1452245);
	offsets[4] = vec3(-0.2535434,  0.4535424,  0.5444254);
	offsets[5] = vec3( 0.3135244, -0.4330132, -0.5422425);
	offsets[6] = vec3( 0.7897852, -0.2524242,  0.4584555);
	offsets[7] = vec3( 0.9792424,  0.0122242, -0.5422161);
	
	float totalOcclusion = 0.0;
	vec4 randCol = texture2D(UserMapSampler, gl_TexCoord[0].xy * RAND_TEXTURE_TILES);

	for(int i = 0;i < SAMPLE_COUNT;++i)
	{
		// Reflect the random offset using the random user texture.
		
		vec3 randVec = reflect(normalize(offsets[i]), randCol.xyz * 2.0 - vec3(1.0, 1.0, 1.0)) * SSAO_REACH;
		vec4 newWorldPos = vec4(currentWPos + randVec, 1.0);
		
		// Calculate our sample depth.
		float sampleDepth = length(newWorldPos.xyz - gl_TexCoord[1].xyz);
	
		// Project the position to the screen.
		newWorldPos = mViewProj * newWorldPos;
		newWorldPos.xy = vec2(newWorldPos.x, newWorldPos.y) / newWorldPos.w * 0.5 + vec2(0.5, 0.5);

		sampleDepth = sampleDepth / lLength - DEPTH_ALIASING_EPISILON;

		// Read from the projected position.
		float newDepth = getDepthAt(clamp(newWorldPos.xy, vec2(0.001, 0.001), vec2(0.999, 0.999)));

		// Compare the two depth samples.
		float depthCalc = newDepth < sampleDepth ? 1.0 : 0.0;
		depthCalc *= clamp(1.0 - (sampleDepth - newDepth) / MAX_DEPTH_DECLINE, 0.0, 1.0);
		totalOcclusion += depthCalc;
	}
	
	totalOcclusion /= float(SAMPLE_COUNT);
	
	gl_FragColor = vec4(1.0 - totalOcclusion * SSAO_OUTPUT_MULTIPLIER);
}
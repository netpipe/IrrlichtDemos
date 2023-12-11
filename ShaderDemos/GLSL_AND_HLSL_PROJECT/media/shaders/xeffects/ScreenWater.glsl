uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;
uniform sampler2D UserMapSampler;

float getDepthAt(vec2 coords)
{
	vec4 texDepth = texture2D(DepthMapSampler, coords);
	return texDepth.r;
}

#define TEST_EPISILON 1.0

#define WATER_COLOR vec4(0.2, 0.8, 1.0, 1.0);
#define SPECULAR_COLOR vec4(0.6, 0.6, 0.6, 0.6)
#define SPECULAR_EXPONENT 32.0

void main()
{
	vec2 TexCoords = gl_TexCoord[0].xy;
	vec3 LStart = gl_TexCoord[1].xyz;
	vec3 LEnd = gl_TexCoord[2].xyz;

	vec3 lVec = LEnd - LStart;
	vec3 lDir = normalize(lVec);
	float lLength = length(lVec);
	float currentDepth = getDepthAt(TexCoords) * lLength;
	vec3 currentWPos = LStart + currentDepth * lDir;
	
	vec4 screenColor = texture2D(ScreenMapSampler, TexCoords.xy);
	
	vec3 planeNormal = vec3(0.0, LStart.y < 0.0 ? -1.0 : 1.0, 0.0);
	float planeDP = dot(planeNormal, lDir);
	
	if(LStart.y < 0.0)
		screenColor *= WATER_COLOR;
	
	vec4 finalColour = screenColor;

	if(planeDP < 0.0)
	{
		float planeDepth = -dot(planeNormal, LStart) / planeDP;

		if(planeDepth < currentDepth && planeDepth < lLength)
		{
			vec3 planePos = LStart + lDir * planeDepth;
		
			float heightDiff = sin(planePos.x / 20.0) + sin(planePos.z / 20.0);
			planePos.y += heightDiff;

			vec3 lightNormal = texture2D(UserMapSampler, planePos.xz / 50.0).xyz - vec3(0.5, 0.5, 0.5);
			lightNormal = normalize(lightNormal);
			vec2 refCoords = clamp(TexCoords.xy + lightNormal.xy / 20.0, vec2(0.01, 0.01), vec2(0.99, 0.99));
			float refDepth = getDepthAt(refCoords) * lLength;
			
			finalColour = WATER_COLOR;

			if(refDepth - TEST_EPISILON > planeDepth)
				finalColour *= texture2D(ScreenMapSampler, refCoords);
			else
				finalColour *= screenColor;

			vec3 sunDirection = vec3(1.0, 1.0, 1.0);
			float diffuse = dot(lightNormal, sunDirection);
			vec3 Reflect = normalize(2.0 * diffuse * lightNormal - sunDirection);
			vec4 specular = pow(clamp(dot(Reflect, lDir), 0.0, 1.0), SPECULAR_EXPONENT) * SPECULAR_COLOR;
			finalColour += specular;

			//vec4 foamColour = vec4(0.8, 0.8, 0.8, 0.8) * diffuse;			
			//float foamFac = clamp(((currentDepth - planeDepth - 3.0) / 5.0), 0.0, 1.0);
			//finalColour = mix(finalColour + foamColour, finalColour, foamFac);

			float shoreFac = clamp((currentDepth - planeDepth) / 20.0, 0.0, 1.0);

			finalColour = mix(screenColor, finalColour, shoreFac);
			
			float fadeFac = clamp(1.0 - (planeDepth - lLength / 10.0) / lLength, 0.0, 1.0);
			finalColour = mix(screenColor, finalColour, fadeFac);
		}
	}
	
	gl_FragColor = finalColour;
}
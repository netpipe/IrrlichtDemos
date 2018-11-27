uniform sampler2D ShadowMapSampler;
uniform vec4 LightColour;
varying float lightVal;

float testShadow(vec2 smTexCoord, vec2 offset, float realDistance)
{
	vec4 texDepth = texture2D( ShadowMapSampler, vec2( smTexCoord + offset));
	float extractedDistance = (texDepth.r + texDepth.g / 256.0);
	
	return (extractedDistance <= realDistance) ? (1.0  / float(SAMPLE_AMOUNT)) : 0.0;
}

vec2 offsetArray[16];

vec4 pixelMain(in vec4 SMPos, in vec4 MVar, in vec2 TexCoords)
{
	offsetArray[0] = vec2(0.0, 0.0);
	offsetArray[1] = vec2(0.0, 1.0);
	offsetArray[2] = vec2(1.0, 1.0);
	offsetArray[3] = vec2(-1.0, -1.0);
	offsetArray[4] = vec2(-2.0, 0.0);
	offsetArray[5] = vec2(0.0, -2.0);
	offsetArray[6] = vec2(2.0, -2.0);
	offsetArray[7] = vec2(-2.0, 2.0);
	offsetArray[8] = vec2(3.0, 0.0);
	offsetArray[9] = vec2(0.0, 3.0);
	offsetArray[10] = vec2(3.0, 3.0);
	offsetArray[11] = vec2(-3.0, -3.0);
	offsetArray[12] = vec2(-4.0, 0.0);
	offsetArray[13] = vec2(0.0, -4.0);
	offsetArray[14] = vec2(4.0, -4.0);
	offsetArray[15] = vec2(-4.0, 4.0);

    SMPos.xy  = SMPos.xy / SMPos.w / 2.0 + vec2(0.5, 0.5);

	vec4 finalCol = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 clampedSMPos = clamp(SMPos.xy, vec2(0.0, 0.0), vec2(1.0, 1.0));
	float lightFactor = 1.0;
	float realDist = MVar.x / MVar.z - 0.002;

	// If this point is within the light's frustum.
    if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar.z)
	{
		for(int i = 0;i < SAMPLE_AMOUNT; i++)
			lightFactor -= testShadow(SMPos.xy, offsetArray[i] * MVar.w, realDist);

		// Multiply with diffuse.
##ifdef ROUND_SPOTLIGHTS
		finalCol = LightColour * lightFactor * MVar.y * clamp(5.0 - 10.0 * length(SMPos.xy - vec2(0.5, 0.5)), 0.0, 1.0);
##else
		finalCol = LightColour * lightFactor * MVar.y;
##endif
	}

	return finalCol;
}

// Main entry point.
void main() 
{
	gl_FragColor = pixelMain(vec4(gl_TexCoord[0]), vec4(gl_TexCoord[1]), vec2(gl_TexCoord[2].xy));
}
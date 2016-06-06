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
	offsetArray[0] = vec2(0.0,0.0);
	offsetArray[1] = vec2(0.0,1.0 / float(MAPRES));
	offsetArray[2] = vec2(1.0 / float(MAPRES),1.0 / float(MAPRES));
	offsetArray[3] = vec2(-1.0 / float(MAPRES),-1.0 / float(MAPRES));
	offsetArray[4] = vec2(-2.0 / float(MAPRES),0.0);
	offsetArray[5] = vec2(0.0,-2.0 / float(MAPRES));
	offsetArray[6] = vec2(2.0 / float(MAPRES),-2.0 / float(MAPRES));
	offsetArray[7] = vec2(-2.0 / float(MAPRES),2.0 / float(MAPRES));
	offsetArray[8] = vec2(3.0 / float(MAPRES),0.0);
	offsetArray[9] = vec2(0.0,3.0 / float(MAPRES));
	offsetArray[10] = vec2(3.0 / float(MAPRES),3.0 / float(MAPRES));
	offsetArray[11] = vec2(-3.0 / float(MAPRES),-3.0 / float(MAPRES));
	offsetArray[12] = vec2(-4.0 / float(MAPRES),0.0);
	offsetArray[13] = vec2(0.0,-4.0 / float(MAPRES));
	offsetArray[14] = vec2(4.0 / float(MAPRES),-4.0 / float(MAPRES));
	offsetArray[15] = vec2(-4.0 / float(MAPRES),4.0 / float(MAPRES));

    SMPos.xy  = SMPos.xy / SMPos.w / 2.0 + vec2(0.5, 0.5);

	vec4 finalCol = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 clampedSMPos = clamp(SMPos.xy, vec2(0.0, 0.0), vec2(1.0, 1.0));
	float lightFactor = 1.0;
	float realDist = MVar.x / MVar.z - 0.002;

	// If this point is within the light's frustum.
    if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar.z)
	{
		for(int i = 0;i < SAMPLE_AMOUNT; i++)
			lightFactor -= testShadow(SMPos.xy, offsetArray[i], realDist);

		// Multiply with diffuse.
		finalCol = LightColour * lightFactor * MVar.y;
	}

	return finalCol;
}

// Main entry point.
void main() 
{
	gl_FragColor = pixelMain(vec4(gl_TexCoord[0]), vec4(gl_TexCoord[1]), vec2(gl_TexCoord[2].xy));
}
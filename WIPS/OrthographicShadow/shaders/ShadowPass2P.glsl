float xsat( float x) 
{
  return clamp( x, 0.0, 1.0);
}

uniform sampler2D ColoredTextureSampler;

##ifdef 2_LAYER
uniform sampler2D DetailMap;
##endif

uniform sampler2D ShadowMapSampler;
varying float lightVal;

float testShadow(vec2 smTexCoord, vec2 offset, float realDistance, float darkness)
{
	vec4 texDepth = texture2D( ShadowMapSampler, vec2( smTexCoord + offset));
	float extractedDistance = (texDepth.r + texDepth.g / 256.0f);

	return (extractedDistance <= realDistance) ? ( darkness / 2.0000 ) : ( 0.000000 );
}

vec2 offsetArray[16];

vec4 pixelMain( in vec4 SMPos, in vec4 MVar, in vec2 TexCoords, in vec2 TexCoords2)
{
	offsetArray[0] = vec2(0.0,0.0);
	offsetArray[1] = vec2(0.0,1.0 / float(float(MAPRES)));
	offsetArray[2] = vec2(1.0 / float(float(MAPRES)),1.0 / float(float(MAPRES)));
	offsetArray[3] = vec2(-1.0 / float(float(MAPRES)),-1.0 / float(MAPRES));
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


    SMPos.x  = (((SMPos.x  / SMPos.w ) / 2.00000) + 0.500000);
    SMPos.y  = (((SMPos.y  / SMPos.w ) / 2.00000) + 0.500000);
					
    if(xsat( SMPos.x  ) == SMPos.x && xsat( SMPos.y  ) == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar.z)
	{		
		vec2 shadCoord = vec2(SMPos.x,SMPos.y);
				
		for(int i = 0;i < SAMPLE_AMOUNT ; i++)
			MVar.y -= testShadow(shadCoord, offsetArray[i],MVar.x , MVar.w) / float(SAMPLE_AMOUNT);
	}
	
##ifdef 2_LAYER
    return (texture2D( ColoredTextureSampler, TexCoords) * texture2D( DetailMap, TexCoords2) * MVar.y);
##else
	return (texture2D( ColoredTextureSampler, TexCoords) * MVar.y);
##endif
}

// Main entry point.
void main() 
{
    vec4 finalCOl = pixelMain( vec4(gl_TexCoord[0]), vec4(gl_TexCoord[1]), vec2(gl_TexCoord[2]), vec2(gl_TexCoord[3]));

    gl_FragData[0] = finalCOl;
}



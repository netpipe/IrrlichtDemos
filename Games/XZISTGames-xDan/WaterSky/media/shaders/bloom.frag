
/* NOTES

- there are lots of places this can be tweaked... So it is not controlled with uniform variables.
- additive blending (EMT_TRANSPARENT_ADD_COLOR) should be enabled in irrlicht
- for more impressive bloom, need a lot more blur samples. But low end cards will not handle this.
  A solution would be to use more passes.

*/

uniform sampler2D texture0;
uniform int pass; // 0 or 1
varying vec2 texCoord02D;

// PARAMETERS

// Brightness (filter)
const float CUTOFF = 0.5; 		// Only colours brighter than this are used in bloom

// Blurring
const int BLUR_SAMPLES = 4;		// Number of positions to sample
const float BLUR_STEP = 0.003;	// Step between samples (as a fraction of render target size)


vec4 brightFilter(vec4 col)
{
	return max(vec4(0.0), col - CUTOFF);
}

void main(void)
{
	vec2 blurOffset;
	
	// Blur horizontally on 1st pass, vertically on 2nd.
	if (pass == 0)
	{
		blurOffset = vec2(BLUR_STEP, 0.0);
	}
	else
	{
		blurOffset = vec2(0.0, BLUR_STEP);
	}
	
	// This should be Gaussian values...?
	float pixelWeight = 1.0 / (1.0 + 2.0 * float(BLUR_SAMPLES));
	
	gl_FragColor = brightFilter( texture2D(texture0, texCoord02D) );
	
	// Blur. Either horizontal or vertical.
	// Start at 1 as centre pixel has already been used above
	for (int i = 1; i < BLUR_SAMPLES+1; i ++)
	{
		float fi = float(i);
		float c = 2.0/fi;
		
		gl_FragColor
			+= brightFilter( texture2D(texture0, texCoord02D + blurOffset*fi) *c )
			+ brightFilter( texture2D(texture0, texCoord02D - blurOffset*fi) *c );
	}
	
	gl_FragColor *= pixelWeight;
	
	// Some hackery tweakery to increase brightness
	// This pixel has been identified as bright in the bright filter so we
	// want to make it REALLY bright
	vec4 inc = 1.0 + gl_FragColor;
	gl_FragColor *= inc*inc*inc;
}

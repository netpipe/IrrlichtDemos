uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

float luminance(vec4 c)
{
	return dot( c, vec4(0.3, 0.59, 0.11, 0.0) );
}

void main() 
{		
vec4 finalCol = texture2D(ColorMapSampler,gl_TexCoord[0].xy);
vec4 blurredCol = texture2D(ScreenMapSampler,gl_TexCoord[0].xy);

gl_FragColor = 	( finalCol + blurredCol * 1.8 ) / 2 +
				luminance(blurredCol) * 
				vec4( 0.5, 0.5, 0.5, 0) +
				luminance(finalCol) *
				vec4( 0.3, 0.3, 0.3, 0);
}
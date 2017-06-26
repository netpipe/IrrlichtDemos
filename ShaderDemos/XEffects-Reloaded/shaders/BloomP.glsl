uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

float luminance(vec4 c)
{
	return dot( c, vec4(0.3, 0.59, 0.11, 0.0) );
}

void main() 
{		
	vec4 bloomCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = (screenCol * 0.9 + bloomCol * 0.5);
}
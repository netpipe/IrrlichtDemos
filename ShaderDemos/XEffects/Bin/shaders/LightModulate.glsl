uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

void main() 
{		
	vec4 finalCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	vec4 lightCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = finalCol * lightCol;
}
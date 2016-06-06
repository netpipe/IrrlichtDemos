uniform sampler2D ColorMapSampler;

void main() 
{		
	vec4 finalCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	gl_FragColor = finalCol;
}
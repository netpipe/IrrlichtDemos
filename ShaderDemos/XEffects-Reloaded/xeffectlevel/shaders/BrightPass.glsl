uniform sampler2D ColorMapSampler;

void main() 
{		
	vec4 finalVal = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	finalVal *= 2.0;
	finalVal = finalVal * finalVal * finalVal;

	gl_FragColor = finalVal;
}
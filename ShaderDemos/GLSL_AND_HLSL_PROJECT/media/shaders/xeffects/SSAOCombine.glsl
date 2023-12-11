uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

void main() 
{		
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);
	vec4 SSAOCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = (screenCol * SSAOCol);
}
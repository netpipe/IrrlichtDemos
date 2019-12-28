uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float EffectStrength;

void main()
{
	// lerp used, using mix instead
	gl_FragColor = mix(
		texture2D(texture0, gl_TexCoord[0].xy), 
		texture2D(texture1, gl_TexCoord[0].xy),	
		EffectStrength);
}

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float EffectStrength;

void main()
{
	vec4 col0 = texture2D(texture1, gl_TexCoord[0].xy);	// Bloom color
	vec4 col1 = texture2D(texture0, gl_TexCoord[0].xy);	// Scene

	// Tonemap (using photographic exposure mapping)
	vec4 col2 = 1.0-exp2(-EffectStrength*col0);

	gl_FragColor = col2+col1;
}
uniform sampler2D texture0;
uniform float EffectStrength;

void main()
{
	vec4 finalCol = texture2D(texture0, gl_TexCoord[0].xy);
	vec4 col0 = finalCol;
	
	// higher contrast
	finalCol.rgb = vec3(pow(float(col0.rgb), EffectStrength));
	
	// mix colors
	finalCol.rgb *= vec3(col0);
	finalCol.rgb += vec3(col0);

	gl_FragColor = finalCol;
}

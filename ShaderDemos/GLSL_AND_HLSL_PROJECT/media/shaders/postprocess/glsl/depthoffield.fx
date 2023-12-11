uniform sampler2D texture0; //SceneBuffer
uniform sampler2D texture1; //BlurBuffer
uniform sampler2D texture2; //DistanceBuffer

void main ()
{
	vec4 sharp = texture2D(texture0, gl_TexCoord[0].xy);
	vec4 blur  = texture2D(texture1, gl_TexCoord[0].xy);
	vec4 dist  = texture2D(texture2, gl_TexCoord[0].xy);
	float factor = 0;
	
	if (dist.a < 0.05)
		factor = 1.0;
	else if(dist.a < 0.1)
		factor = 20.0 *(0.1 - dist.a);
	else if(dist.a < 0.5)
		factor=0;
	else
		factor = 2.0 *(dist.a - 0.5);

	clamp(factor, 0.0, 0.90);
	//lerp(sharp, blur, factor);
	gl_FragColor = mix(sharp, blur, factor);
}

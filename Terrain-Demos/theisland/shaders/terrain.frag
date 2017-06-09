uniform sampler2D splat1 : register(s0); 
uniform sampler2D splat2 : register(s1); 

void main()
{
	vec4 TexColor = lerp(texture2D( splat1, gl_TexCoord[0].st),texture2D( splat2, gl_TexCoord[0].st),gl_TexCoord[1].x);
	//vec4 TexColor = texture2D( splat2, gl_TexCoord[0].st);
	gl_FragColor = gl_Color* TexColor;
}

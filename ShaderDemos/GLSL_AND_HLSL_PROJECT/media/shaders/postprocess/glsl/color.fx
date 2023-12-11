uniform sampler2D texture0;
uniform float ColorR;
uniform float ColorG;
uniform float ColorB;

void main()
{
	vec4 texCol = texture2D(texture0, gl_TexCoord[0].xy);
	
	// digital ITU R 709: Y = 0.2126 R + 0.7152 G + 0.0722 B 
	// digital ITU R 601: Y = 0.2990 R + 0.5870 G + 0.1140 B
	float lum = dot(texCol.rgb, vec3(0.2990, 0.5870, 0.1140));	
	gl_FragColor = vec4(
		pow(lum, 1.0/ColorR),
		pow(lum, 1.0/ColorG),
		pow(lum, 1.0/ColorG), 
		texCol.a)*ColorB+texCol*(1.0-ColorB);
}

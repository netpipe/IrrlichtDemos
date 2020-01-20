uniform sampler2D color;
uniform sampler2D SHatlas2;

void main()
{
	gl_FragColor = vec4(pow(texture2D(color,gl_TexCoord[0].xy).rgb,0.92)*1.5*(texture2D(SHatlas2,gl_TexCoord[0].xy).rgb+0.08),1.0);
}

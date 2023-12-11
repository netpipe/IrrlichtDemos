uniform sampler2D texture0;

void main()
{
	vec4 finalCol = vec4(1.0-texture2D(texture0, gl_TexCoord[0].xy).rgb, 1.0);
	gl_FragColor = finalCol;
}

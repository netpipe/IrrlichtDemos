// based on http://www.geeks3d.com/20091112/shader-library-dream-vision-post-processing-filter-glsl/

uniform sampler2D texture0;

void main()
{
	vec4 finalCol = texture2D(texture0, gl_TexCoord[0].xy);

	finalCol += texture2D(texture0, gl_TexCoord[0].xy+0.001);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy+0.003);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy+0.005);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy+0.007);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy+0.009);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy+0.011);

	finalCol += texture2D(texture0, gl_TexCoord[0].xy-0.001);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy-0.003);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy-0.005);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy-0.007);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy-0.009);
	finalCol += texture2D(texture0, gl_TexCoord[0].xy-0.011);
	finalCol = finalCol/9.5;
	gl_FragColor = finalCol;
}




uniform sampler2D texture0;
uniform float BufferWidth;
uniform float BufferHeight;

void main()
{
	gl_FragColor=texture2D(texture0, gl_TexCoord[0].xy);
}

void downscale2x2()
{
	vec2 texSize = vec2(1.0/BufferWidth, 1.0/BufferHeight);
	vec4 average = vec4(0.0);
	vec2 samples[4];
	samples[0]=vec2(-0.5, -0.5);
	samples[1]=vec2(-0.5,  0.5);
	samples[2]=vec2( 0.5, -0.5);
	samples[3]=vec2( 0.5,  0.5);
	for (int i=0; i<4; ++i)
	{
		average += texture2D(texture0, gl_TexCoord[0].xy+texSize*samples[i]);
	}
	gl_FragColor = average*0.25;
}

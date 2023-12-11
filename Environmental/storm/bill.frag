uniform sampler2D tex;
uniform float alpha;
uniform float time;

void main()
{
	float s = gl_TexCoord[0].s + sin(time * 0.5);
	float t = gl_TexCoord[0].t;
	vec2 tc = vec2(s, t);
	
	vec4 texcolor = texture2D(tex, tc);
	texcolor *= alpha;
	//texcolor.a = alpha;
	gl_FragColor = texcolor;
	

}



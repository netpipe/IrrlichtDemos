uniform sampler2D tex;
uniform float alpha;
uniform float time;

void main()
{
	//float s = mod(gl_TexCoord[0].s + time * 0.8, 1.0);
	float s = gl_TexCoord[0].s + sin(time * 0.5);
	//float s = gl_TexCoord[0].s;
	
	//float t = mod(gl_TexCoord[0].t + time * 0.8, 1.0); // vertical dir
	float t = gl_TexCoord[0].t - time * 0.2;
	//float t = gl_TexCoord[0].t;
	
	//while (t > 1.0)
		//t -= 1.0;
	
	//vec4 tc = gl_TexCoord[0];
	//tc.s = s;
	//tc.t = t;
	
	vec2 tc = vec2(s, t);
	//vec4 texcolor = texture2D(tex, gl_TexCoord[0].st);
	//vec4 texcolor = texture2D(tex, tc.st);
	vec4 texcolor = texture2D(tex, tc);
	//texcolor.a = alpha;
	texcolor *= alpha;
	gl_FragColor = texcolor;
	
	//gl_FragColor = texcolor * alpha;
	
	//gl_FragColor = texcolor * (1.0f - gl_Color.a);
	//gl_FragColor = texcolor * gl_Color.a;
	
	// NB: 1.0f => illegal! 
	//texcolor.a = 1.0 - texcolor.a;
	
	//gl_FragColor = texcolor;
	
	
}



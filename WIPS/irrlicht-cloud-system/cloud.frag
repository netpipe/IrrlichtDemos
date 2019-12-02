#version 130

uniform sampler2D texture;
varying vec4 vertex;
varying float scale;

#define NUM_LIGHTS gl_MaxLights

void main()
{
	float color = texture2D(texture, gl_TexCoord[0].st).r;
	vec3 light = vec3(0);
	vec4 lv;
	int i=0;
	for (i; i<NUM_LIGHTS; i++)
	{
		lv = gl_LightSource[i].position - vertex;
		light += vec3(gl_LightSource[i].diffuse) / length(lv);
	}
	light = 0.5*light * (1.2 - 0.5*color);
	gl_FragColor = vec4(light, color * min(1, 0.1*length(vertex)-0.1)); // there is a 0.1 offset to ajust cloud on screen sides
}

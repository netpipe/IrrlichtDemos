uniform sampler2D tex0;

varying vec3 Light;
varying vec3 Norm;
varying vec3 View;
varying vec2 TexCo;

void main()
{
	vec4 diffuse = texture2D(tex0, TexCo);
	float diff = clamp(dot( Norm, Light), 0.0, 1.0);
	vec3 Reflect = (((2.0 * diff) * Norm) - Light);
	vec4 specular = vec4(pow(clamp(dot(Reflect, View), 0.0, 1.0), 8.0));
	gl_FragColor = diffuse * diff + specular;
}
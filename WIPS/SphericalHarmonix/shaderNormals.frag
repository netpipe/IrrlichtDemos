// normal encoding

varying vec3 normal;

void main()
{
	vec3 Normal = normalize(normal);
	gl_FragColor = vec4(Normal*0.5+0.5,1.0);
}

/*Shader for Open GL*/

uniform sampler2D baseMap;
uniform sampler2D reflectionMap;
uniform float mipsCount;

varying vec2 Texcoord;
varying vec3 Normal;
varying vec3 ViewDirection;

vec2 sphere2uv(vec3 vector)
{
   vec2 o;
   float PI = 3.14159265358979323846;
   //Actually this differs from the original sources in the signs of the vector, this way we get actually a reflected result :)
   o.x = 0.5+atan2(-vector.z,-vector.x)/(2.0*PI);
   o.y = 0.5-asin(vector.y)/PI;
   return o;
}

void main()
{
	vec3 reflection = normalize(reflect(ViewDirection,Normal));
	vec2 uv = sphere2uv(reflection);
	float gloss = texture(baseMap,Texcoord).y;
	vec3 col = textureLod(reflectionMap,uv,(1.0f-gloss)*mipsCount).xyz;
	gl_FragColor = vec4(col,1);
}